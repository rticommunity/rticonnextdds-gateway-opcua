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

#include <rti/ddsopcua/requester/detail/DdsRequesterOperations.hpp>

#include "conversion/OpcUaBuiltinDataTypes.hpp"
#include "opcUaSdk/OpcUaSdkDataTypes.hpp"

namespace rti { namespace ddsopcua { namespace requester { namespace impl {

uint32_t read_request(
        rti::opcua::types::Variant& value,
        ServiceRequester& requester,
        const std::string& server_id,
        const opcua::NodeId& node_id,
        const uint32_t timeout_sec)
{
    uint32_t status_code = 1;

    // Create read request based on node_id
    rti::opcua::types::services::ReadRequest read_request;
    read_request.node_id(node_id.node_id());
    read_request.attribute_id(
            rti::opcua::types::AttributeId::ATTRIBUTE_ID_VALUE);

    // Issue a read request to the server identifier by server_id
    rti::opcua::types::services::ServiceRequest service_request;
    service_request.server_id(server_id);
    service_request.request().read_request(read_request);

    // Send request
    rti::core::SampleIdentity request_id =
            requester.send_request(service_request);

    // Wait for corresponding reply
    requester.wait_for_replies(
            1,
            dds::core::Duration(timeout_sec, 0),
            request_id);

    // Process replies
    for (auto reply : requester.take_replies(request_id)) {
        // Print out received sample
        std::cout << rti::topic::to_string(
                reply.data(),
                rti::topic::PrintFormatProperty::Json())
                  << std::endl;
        value = reply.data().reply().read_reply().value();
        return reply.data().status_code();
    }

    return status_code;
}

uint32_t write_request(
        ServiceRequester& requester,
        const std::string& server_id,
        const opcua::NodeId& node_id,
        const rti::opcua::types::Variant& variant,
        const uint32_t timeout_sec)
{
    uint32_t status_code = 1;

    // Create write request using the NodeId and Variant
    rti::opcua::types::services::WriteRequest write_request;
    write_request.node_id(node_id.node_id());
    write_request.attribute_id(
            rti::opcua::types::AttributeId::ATTRIBUTE_ID_VALUE);
    write_request.value().value(variant);

    // Issue a write request to the server identified by server_id
    rti::opcua::types::services::ServiceRequest service_request;
    service_request.server_id("MyServerConnection");
    service_request.request().write_request(write_request);
    rti::core::SampleIdentity request_id =
            requester.send_request(service_request);

    // Wait for corresponding reply
    requester.wait_for_replies(
            1,
            dds::core::Duration(timeout_sec, 0),
            request_id);

    // Process replies
    for (auto reply : requester.take_replies()) {
        std::cout << rti::topic::to_string(
                reply.data(),
                rti::topic::PrintFormatProperty::Json())
                  << std::endl;
        return reply.data().status_code();
    }

    return status_code;
}


}}}}  // namespace rti::ddsopcua::requester::impl
