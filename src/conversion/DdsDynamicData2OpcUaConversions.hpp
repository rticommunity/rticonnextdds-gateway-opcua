/*
 * (c) 2017-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#include <rtiboost/core/enable_if.hpp>
#include <rtiboost/type_traits/is_arithmetic.hpp>

#include <rti/core/constants.hpp>
#include <dds/core/xtypes/DynamicData.hpp>

#include "log/LogMsg.hpp"
#include "conversion/OpcUaBuiltinDataTypes.hpp"
#include "opcUaSdk/OpcUaSdkDataTypes.hpp"

namespace rti { namespace ddsopcua { namespace conversion {

using namespace dds::core::xtypes;
using namespace rti::core::xtypes;
using namespace rtiboost;

template<typename OPCUA_TYPE, typename DDS_TYPE>
void set_opc_ua_value(
        OPCUA_TYPE& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    opcua_value = dd_variant.value<DDS_TYPE>(dd_variant_member_name);
}

template<typename OPCUA_TYPE, typename DDS_TYPE>
void set_opc_ua_value(
        OPCUA_TYPE& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id)
{
    opcua_value = dd_variant.value<DDS_TYPE>(dd_variant_member_id);
}

template<typename OPCUA_TYPE>
void unset_opc_ua_value(OPCUA_TYPE& opcua_value)
{
    // NOOP by default
}

template<>
void set_opc_ua_value<UA_String, std::string>(
        UA_String& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name);

template<>
void set_opc_ua_value<UA_String, std::string>(
        UA_String& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id);

template<>
void unset_opc_ua_value<UA_String>(UA_String& opcua_value);

template<>
void set_opc_ua_value<UA_Guid, opcua::types::Guid>(
        UA_Guid& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name);

template<>
void set_opc_ua_value<UA_Guid, opcua::types::Guid>(
        UA_Guid& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id);

template<>
void set_opc_ua_value<UA_ByteString, opcua::types::ByteString>(
        UA_ByteString& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name);

template<>
void set_opc_ua_value<UA_ByteString, opcua::types::ByteString>(
        UA_ByteString& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id);

template<>
void set_opc_ua_value<UA_NodeId, opcua::types::NodeId>(
        UA_NodeId& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name);

template<>
void set_opc_ua_value<UA_NodeId, opcua::types::NodeId>(
        UA_NodeId& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id);

template<>
void set_opc_ua_value<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        UA_ExpandedNodeId& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name);

template<>
void set_opc_ua_value<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        UA_ExpandedNodeId& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id);

template<>
void set_opc_ua_value<UA_QualifiedName, opcua::types::QualifiedName>(
        UA_QualifiedName& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name);

template<>
void set_opc_ua_value<UA_QualifiedName, opcua::types::QualifiedName>(
        UA_QualifiedName& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id);

template<typename OPCUA_TYPE, typename DDS_TYPE>
void set_variant_scalar(
        opcua::sdk::types::Variant& variant,
        uint32_t type_kind,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    OPCUA_TYPE value;
    set_opc_ua_value<OPCUA_TYPE, DDS_TYPE>(
            value,
            dd_variant,
            dd_variant_member_name);
    UA_Variant_setScalarCopy(
            variant.get(),
            (void*) &value,
            &UA_TYPES[type_kind]);
    unset_opc_ua_value(value);
}

template<typename OPCUA_TYPE, typename DDS_TYPE>
void set_variant_scalar(
        opcua::sdk::types::Variant& variant,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    set_variant_scalar<OPCUA_TYPE, DDS_TYPE>(
            variant,
            dd_variant.discriminator_value() - 1,
            dd_variant,
            dd_variant_member_name);
}

template<typename OPCUA_TYPE, typename DDS_TYPE>
typename enable_if_c<is_arithmetic<DDS_TYPE>::value, void>::type
set_variant_array(
        opcua::sdk::types::Variant& variant,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    std::vector<DDS_TYPE> value;
    dd_variant.get_values<DDS_TYPE>(dd_variant_member_name, value);
    DDS_TYPE* value_ref = &value[0];
    UA_Variant_setArrayCopy(
            variant.get(),
            (void*) value_ref,
            value.size(),
            &UA_TYPES[dd_variant.discriminator_value() - 1]);
}

template<typename OPCUA_TYPE, typename DDS_TYPE>
typename enable_if_c<!is_arithmetic<DDS_TYPE>::value, void>::type
set_variant_array(
        opcua::sdk::types::Variant& variant,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    LoanedDynamicData array = dd_variant.loan_value(dd_variant_member_name);
    uint32_t array_size = array.get().member_count();

    std::vector<OPCUA_TYPE> opcua_value(array_size);
    for (uint32_t i = 0; i < array_size; ++i) {
        set_opc_ua_value<OPCUA_TYPE, DDS_TYPE>(
                opcua_value[i],
                array.get(),
                i + 1);
    }

    UA_Variant_setArrayCopy(
            variant.get(),
            (void*) &opcua_value[0],
            array_size,
            &UA_TYPES[dd_variant.discriminator_value() - 1]);

    for (uint32_t i = 0; i < array_size; ++i) {
        unset_opc_ua_value<OPCUA_TYPE>(opcua_value[i]);
    }
}

void dds_dynamic_data_variant_type_to_opc_ua_variant_scalar(
        opcua::sdk::types::Variant& variant,
        const DynamicData& variant_value,
        const uint32_t variant_value_type);

void dds_dynamic_data_variant_type_to_opc_ua_variant_array(
        opcua::sdk::types::Variant& variant,
        const DynamicData& variant_value,
        const uint32_t variant_value_type);

void dds_dynamic_data_variant_type_to_opc_ua_variant(
        opcua::sdk::types::Variant& variant,
        const DynamicData& data);

void dds_dynamic_data_member_to_dds_dynamic_data_variant(
        DynamicData& variant,
        const DynamicData& data,
        const std::string& member_name);

void dds_dynamic_data_to_opc_ua_guid(
        UA_Guid& opcua_value,
        const DynamicData& dd_guid);

/**
 * @brief Transform an OPC UA NodeId stored in a DynamicData object into a
 * new rti::opcua::sdk::types::NodeId object.
 *
 * @param data DynamicData object containing the OPC UA NodeId value.
 * @return opcua::sdk::types::NodeId Transformed object.
 */
opcua::sdk::types::NodeId dds_dynamic_data_node_id_to_opc_ua_node_id(
        DynamicData& data);

void dds_dynamic_data_node_id_to_opc_ua_node_id(
        UA_NodeId& opcua_value,
        const DynamicData& data);

void dds_dynamic_data_to_opc_ua_expanded_node_id(
        UA_ExpandedNodeId& opcua_value,
        const DynamicData& data);


}}}  // namespace rti::ddsopcua::conversion
