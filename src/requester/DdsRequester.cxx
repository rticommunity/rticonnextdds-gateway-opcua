/*
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
 * All rights reserved.
 *
 * RTI grants Licensee a license to use, modify, compile, and create derivative
 * works of the Software solely in combination with RTI Connext DDS. Licensee
 * may redistribute copies of the Software provided that all such copies are
 * subject to this License. The Software is provided "as is", with no warranty
 * of any type, including any warranty for fitness for any purpose. RTI is
 * under no obligation to maintain or support the Software. RTI shall not be
 * liable for any incidental or consequential damages arising out of the use or
 * inability to use the Software. For purposes of clarity, nothing in this
 * License prevents Licensee from using alternate versions of DDS, provided
 * that Licensee may not combine or link such alternate versions of DDS with
 * the Software.
 */

#include <thread>
#include <chrono>

#include <rti/topic/to_string.hpp>

#include <rti/ddsopcua/requester/DdsRequester.hpp>

#include "conversion/OpcUaBuiltinDataTypes.hpp"

namespace rti { namespace ddsopcua { namespace requester {

Requester::Requester(
        int domain_id,
        const dds::domain::qos::DomainParticipantQos& participant_qos)
        : participant_(domain_id, participant_qos), requester_(dds::core::null)
{
    // We call the service AttributeServiceSet_, that is the prefix
    // of the actual Request and Reply Topic, which the Gateway names
    // AttributeServiceSet_Request and AttributeServiceSet_Reply.
    rti::request::RequesterParams requester_params(participant_);
    requester_params.service_name("AttributeServiceSet_");

    // Load reliable profile with keep last and transient local
    dds::core::QosProvider::Default()->default_profile(
            "BuiltinQosLibExp::Generic.KeepLastReliable.TransientLocal");
    dds::core::QosProvider::Default()->reload_profiles();

    // Enable unbounded sequence support for DDS types
    rti::core::policy::Property property;
    property.set(rti::core::policy::Property::Entry(
            "dds.data_writer.history.memory_manager.fast_pool.pool_buffer_max_"
            "size",
            "4096"));
    dds::pub::qos::DataWriterQos datawriter_qos =
            dds::core::QosProvider::Default().datawriter_qos();
    datawriter_qos.policy<rti::core::policy::Property>(property);
    requester_params.datawriter_qos(datawriter_qos);

    // Create internal DDS requester to communicate with Gateway
    requester_ = rti::request::Requester<
            rti::opcua::types::services::ServiceRequest,
            rti::opcua::types::services::ServiceReply>(requester_params);
}

bool Requester::wait_for_matching_gateway(
        uint32_t timeout_sec,
        const bool& exit_condition)
{
    dds::core::status::PublicationMatchedStatus pub_status;
    uint32_t elapsed_sec = 0;

    // Search for a DataReader matching the request Topic, the exit
    // wait a second and search again until: (1) a matching DataReader
    // is found, (2) the exit condition is triggered (e.g., CTR+C), or
    // (3) there's a timeout.
    do {
        pub_status =
                requester_.request_datawriter().publication_matched_status();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        elapsed_sec++;
    } while (pub_status.current_count() == 0 && !exit_condition
             && timeout_sec > elapsed_sec);

    if (pub_status.current_count() == 0) {
        return false;
    }

    return true;
}

uint32_t Requester::write_request(
        const std::string& server_id,
        const rti::opcua::types::NodeId& node_id,
        const rti::opcua::types::Variant& variant,
        const uint32_t timeout_sec)
{
    return impl::write_request(
            requester_,
            server_id,
            node_id,
            variant,
            timeout_sec);
}

uint32_t Requester::read_request(
        rti::opcua::types::Variant& value,
        const std::string& server_id,
        const rti::opcua::types::NodeId& node_id,
        const uint32_t timeout_sec)
{
    return impl::read_request(
            value,
            requester_,
            server_id,
            node_id,
            timeout_sec);
}

}}}  // namespace rti::ddsopcua::requester
