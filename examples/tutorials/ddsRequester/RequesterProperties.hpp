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

#ifndef RTI_DDSOPCUA_GENERIC_REQUESTER_PROPERTIES_HPP_
#define RTI_DDSOPCUA_GENERIC_REQUESTER_PROPERTIES_HPP_

#include "conversion/OpcUaBuiltinDataTypes.hpp"

namespace rti { namespace ddsopcua { namespace requester {

/**
 * Properties for a the requester that set parameters, such as the the servier
 * identifier within the Gateway, the DDS Domain ID, and the Nodes to read or
 * write
 */
struct RequesterProperties {
    /**
     * @brief Service Operation to be executed; i.e., read or write.
     */
    rti::opcua::types::services::ServiceOperation service_operation;
    /**
     * @brief
     */
    std::string server_id;
    rti::opcua::types::NodeId node_id;
    rti::opcua::types::Variant variant;
    std::string value_str;
    rti::opcua::types::BuiltinTypeKind value_type;
    int dds_domain_id;
    uint32_t iterations;
    uint32_t period_msec;
    uint32_t timeout;

    RequesterProperties()
            : service_operation(rti::opcua::types::services::ServiceOperation::
                                        SERVICE_OPERATION_READ),
              server_id(),
              node_id(),
              variant(),
              value_type(rti::opcua::types::BuiltinTypeKind::DIAGNOSTICINFO_TYPE),
              dds_domain_id(0),
              iterations(1),
              period_msec(0),
              timeout(10)
    {
    }
};

using ServiceOperation = rti::opcua::types::services::ServiceOperation;

}}}  // namespace rti::ddsopcua::requester

#endif  // RTI_DDSOPCUA_GENERIC_REQUESTER_PROPERTIES_HPP_
