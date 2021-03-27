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

#ifndef RTI_DDSOPCUA_REQUESTER_OPCUA_HPP_
#define RTI_DDSOPCUA_REQUESTER_OPCUA_HPP_

#include <cstdint>
#include <string>

namespace rti { namespace opcua { namespace types {

class NodeId;
class Variant;

namespace services {
class ServiceRequest;
class ServiceReply;
}  // namespace services

}}}  // namespace rti::opcua::types

namespace rti { namespace ddsopcua { namespace requester { namespace opcua {

class NodeId {
public:
    NodeId(const rti::opcua::types::NodeId& node_id);

    NodeId(const uint16_t namespace_index, const uint32_t node_identifier);

    NodeId(const uint16_t namespace_index, const std::string& node_identifier);

    ~NodeId();

    rti::opcua::types::NodeId& node_id() const;

private:
    rti::opcua::types::NodeId* node_id_;
};

// class Variant {
// public:

// private:

// };


}}}}  // namespace rti::ddsopcua::requester::opcua

#endif  // RTI_DDSOPCUA_REQUESTER_OPCUA_HPP_