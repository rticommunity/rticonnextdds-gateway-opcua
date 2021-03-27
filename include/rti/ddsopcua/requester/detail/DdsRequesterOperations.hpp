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

#ifndef RTI_DDSOPCUA_GENERIC_REQUESTER_OPERATIONS_HPP_
#define RTI_DDSOPCUA_GENERIC_REQUESTER_OPERATIONS_HPP_

#include <string>

#include <rti/request/Requester.hpp>
#include <rti/topic/to_string.hpp>

#include "OpcUaTypes.hpp"

namespace rti { namespace ddsopcua { namespace requester { namespace impl {

using ServiceRequester = rti::request::Requester<
        rti::opcua::types::services::ServiceRequest,
        rti::opcua::types::services::ServiceReply>;

uint32_t read_request(
        rti::opcua::types::Variant& value,
        ServiceRequester& requester,
        const std::string& server_id,
        const opcua::NodeId& node_id,
        const uint32_t timeout_sec);

uint32_t write_request(
        ServiceRequester& requester,
        const std::string& server_id,
        const opcua::NodeId& node_id,
        const rti::opcua::types::Variant& variant,
        const uint32_t timeout_sec);


}}}}  // namespace rti::ddsopcua::requester::impl

#endif  // RTI_DDSOPCUA_GENERIC_REQUESTER_OPERATIONS_HPP_
