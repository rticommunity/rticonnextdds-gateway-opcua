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

#ifndef RTI_DDSOPCUA_GENERIC_REQUESTER_HPP_
#define RTI_DDSOPCUA_GENERIC_REQUESTER_HPP_

#include <dds/domain/DomainParticipant.hpp>
#include <rti/request/Requester.hpp>

#include "rti/ddsopcua/requester/detail/OpcUaTypes.hpp"
#include "rti/ddsopcua/requester/detail/DdsRequesterOperations.hpp"

namespace rti { namespace ddsopcua { namespace requester {

/**
 * Requester class provides access to a remote OPC UA/DDS Gateway. A Requester
 * can issue read or write requests, which provide the necessary mechanisms
 * to read or write data from a remote OPC UA Server.
 */
class Requester {
public:
    /**
     * @brief Construct a new Requester object
     *
     * @param domain_id DDS Domain ID where the underlying DDS requester will
     * operate.
     * @param participant_qos DDS DomainParticipantQoS for the underlying
     * DomainParticipant the DDS requester uses to operate.
     */
    Requester(
            int domain_id,
            const dds::domain::qos::DomainParticipantQos& participant_qos =
                    dds::domain::qos::DomainParticipantQos());

    /**
     * @brief Wait until the Requester discovers a matching OPC UA/DDS Gateway
     * providing access to remote OPC UA Servers.
     *
     * @param timeout_sec Number of seconds to wait for a matching OPC UA/DDS
     * Gateway.
     * @param exit_condition Boolean parameter that upon activation will cause
     * the internal loop that checks whether the Gateway has been discovered
     * to exit.
     * @return true A Gateway has been discovered.
     * @return false No Gateway has been discovered.
     */
    bool wait_for_matching_gateway(
            uint32_t timeout_sec = 10,
            const bool& exit_condition = false);

    /**
     * @brief Performs a read request on a remote OPC UA Server through an
     * OPC UA/DDS Gateway. Users must provide the ID identifying the remote
     * OPC UA Server within the Gateway, the Node ID identifying the Node to
     * be read, and optionally a timeout. This method prints the JSON
     * representation of the information read from the remote system.
     *
     * @param value Variant containing the read value.
     * @param server_id String that identifies the remote OPC UA Server in the
     * OPC UA/DDS Gateway configuration (see opcua_connection_ref set within the
     * <service_set> tag of the opcua_to_dds_bridge configuration).
     * @param node_id Identifier of the Node to be Read.
     * @param timeout_sec Seconds to wait for a response before giving up.
     * @return Return code indicating the success or failure of the operation.
     */
    uint32_t read_request(
            rti::opcua::types::Variant& value,
            const std::string& server_id,
            const rti::opcua::types::NodeId& node_id,
            const uint32_t timeout_sec = 10);

    /**
     * @brief Performs a write request on a remote OPC UA Server through an
     * OPC UA/DDS Gateway. Users must provide the ID identifying the remote
     * OPC UA Server within the Gateway, the Node ID identifying the Node to
     * be written, the value to be written in the form of an OPC UA Variant,
     * and optionally a timeout. Upon completion, the method prints out the
     * JSON representation of the response issued by the OPC UA/DDS Gateway.
     *
     * @param server_id String that identifies the remote OPC UA Server in the
     * OPC UA/DDS Gateway configuration (see opcua_connection_ref set within the
     * <service_set> tag of the opcua_to_dds_bridge configuration).
     * @param node_id Identifier of the Node to be Read.
     * @param variant Representation of the value to be written in the form of
     * an OPC UA Variant.
     * @param timeout_sec Seconds to wait for a response before giving up.
     * @return Return code indicating the success or failure of the operation.
     */
    uint32_t write_request(
            const std::string& server_id,
            const rti::opcua::types::NodeId& node_id,
            const rti::opcua::types::Variant& variant,
            const uint32_t timeout_sec = 10);

    /**
     * @brief Initializes DDS Globals and necessary memory allocation.
     */
    static void initialize_globals()
    {
    }

    /**
     * @brief Initializes DDS globals and performs necessary memory
     * deallocation.
     */
    static void finalize_globals()
    {
        dds::domain::DomainParticipant::finalize_participant_factory();
    }

private:
    dds::domain::DomainParticipant participant_;
    rti::request::Requester<
            rti::opcua::types::services::ServiceRequest,
            rti::opcua::types::services::ServiceReply>
            requester_;
};

}}}  // namespace rti::ddsopcua::requester

#endif  // RTI_DDSOPCUA_GENERIC_REQUESTER_HPP_
