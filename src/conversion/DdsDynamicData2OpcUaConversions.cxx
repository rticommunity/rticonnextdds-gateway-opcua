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

#include <rti/core/constants.hpp>

#include "log/LogMsg.hpp"
#include "conversion/OpcUaBuiltinDataTypes.hpp"
#include "DdsDynamicData2OpcUaConversions.hpp"
#include "DdsDynamicDataUtils.hpp"

namespace rti { namespace ddsopcua { namespace conversion {

void dds_dynamic_data_to_opc_ua_guid(
        UA_Guid& opcua_value,
        const DynamicData& dd_guid)
{
    opcua_value.data1 = dd_guid.value<uint32_t>("data1");
    opcua_value.data2 = dd_guid.value<uint16_t>("data2");
    opcua_value.data3 = dd_guid.value<uint16_t>("data3");
    std::vector<uint8_t> data4;
    dd_guid.get_values("data4", data4);
    std::copy(data4.begin(), data4.end(), opcua_value.data4);
}

void dds_dynamic_data_to_opc_ua_node_id(
        UA_NodeId& opcua_value,
        const DynamicData& data)
{
    DynamicData& variant_value = const_cast<DynamicData&>(data);
    uint16_t namespace_index = variant_value.value<uint16_t>("namespace_index");
    LoanedDynamicData identifier_type =
            variant_value.loan_value("identifier_type");

    switch (static_cast<opcua::types::NodeIdentifierKind>(
            identifier_type.get().discriminator_value())) {
    case opcua::types::NodeIdentifierKind::NUMERIC_NODE_ID: {
        opcua_value = UA_NODEID_NUMERIC(
                namespace_index,
                identifier_type.get().value<uint32_t>("numeric_id"));
    } break;
    case opcua::types::NodeIdentifierKind::STRING_NODE_ID: {
        opcua_value = UA_NODEID_STRING_ALLOC(
                namespace_index,
                const_cast<char*>(identifier_type.get()
                                          .value<std::string>("string_id")
                                          .c_str()));
    } break;
    case opcua::types::NodeIdentifierKind::GUID_NODE_ID: {
        UA_Guid guid;
        LoanedDynamicData dd_guid = identifier_type.get().loan_value("guid_id");
        dds_dynamic_data_to_opc_ua_guid(guid, dd_guid.get());
        opcua_value = UA_NODEID_GUID(namespace_index, guid);
    } break;
    case opcua::types::NodeIdentifierKind::OPAQUE_NODE_ID: {
        std::vector<uint8_t> dds_bytestring;
        identifier_type.get().get_values("opaque_id", dds_bytestring);
        std::string bytestring_string(
                dds_bytestring.begin(),
                dds_bytestring.end());
        opcua_value = UA_NODEID_BYTESTRING_ALLOC(
                namespace_index,
                const_cast<char*>(bytestring_string.c_str()));
    } break;
    }
}

void dds_dynamic_data_to_opc_ua_expanded_node_id(
        UA_ExpandedNodeId& opcua_value,
        const DynamicData& data)
{
    DynamicData& variant_value = const_cast<DynamicData&>(data);
    uint16_t namespace_index = variant_value.value<uint16_t>("namespace_index");
    LoanedDynamicData identifier_type =
            variant_value.loan_value("identifier_type");

    switch (static_cast<opcua::types::NodeIdentifierKind>(
            identifier_type.get().discriminator_value())) {
    case opcua::types::NodeIdentifierKind::NUMERIC_NODE_ID: {
        opcua_value = UA_EXPANDEDNODEID_NUMERIC(
                namespace_index,
                identifier_type.get().value<uint32_t>("numeric_id"));
    } break;
    case opcua::types::NodeIdentifierKind::STRING_NODE_ID: {
        opcua_value = UA_EXPANDEDNODEID_STRING_ALLOC(
                namespace_index,
                const_cast<char*>(identifier_type.get()
                                          .value<std::string>("string_id")
                                          .c_str()));
    } break;
    case opcua::types::NodeIdentifierKind::GUID_NODE_ID: {
        UA_Guid guid;
        LoanedDynamicData dd_guid = identifier_type.get().loan_value("guid_id");
        dds_dynamic_data_to_opc_ua_guid(guid, dd_guid.get());
        opcua_value = UA_EXPANDEDNODEID_STRING_GUID(namespace_index, guid);
    } break;
    case opcua::types::NodeIdentifierKind::OPAQUE_NODE_ID: {
        std::vector<uint8_t> dds_bytestring;
        identifier_type.get().get_values("opaque_id", dds_bytestring);
        std::string bytestring_string(
                dds_bytestring.begin(),
                dds_bytestring.end());
        opcua_value = UA_EXPANDEDNODEID_BYTESTRING_ALLOC(
                namespace_index,
                const_cast<char*>(bytestring_string.c_str()));
    } break;
    }

    opcua_value.nodeId.namespaceIndex =
            variant_value.value<uint16_t>("namespace_index");
    opcua_value.namespaceUri = UA_STRING_ALLOC(
            variant_value.value<std::string>("namespace_uri").c_str());
    opcua_value.serverIndex = variant_value.value<uint32_t>("server_index");
}

template<>
void set_opc_ua_value<UA_String, std::string>(
        UA_String& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_member_name_name)
{
    opcua_value = UA_STRING_ALLOC(
            dd_variant.value<std::string>(dd_member_name_name).c_str());
}

template<>
void set_opc_ua_value<UA_String, std::string>(
        UA_String& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_member_name_id)
{
    opcua_value = UA_STRING_ALLOC(
            dd_variant.value<std::string>(dd_member_name_id).c_str());
}

template<>
void set_opc_ua_value<UA_Guid, opcua::types::Guid>(
        UA_Guid& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_member_name_name)
{
    LoanedDynamicData dd_guid = dd_variant.loan_value(dd_member_name_name);
    dds_dynamic_data_to_opc_ua_guid(opcua_value, dd_guid.get());
}

template<>
void unset_opc_ua_value<UA_String>(UA_String& opcua_value)
{
    UA_String_deleteMembers(&opcua_value);
}

template<>
void set_opc_ua_value<UA_Guid, opcua::types::Guid>(
        UA_Guid& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_member_name_id)
{
    LoanedDynamicData dd_guid = dd_variant.loan_value(dd_member_name_id);
    dds_dynamic_data_to_opc_ua_guid(opcua_value, dd_guid.get());
}

template<>
void set_opc_ua_value<UA_ByteString, opcua::types::ByteString>(
        UA_ByteString& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_member_name_name)
{
    std::vector<uint8_t> bytestring;
    dd_variant.get_values(dd_member_name_name, bytestring);
    std::string bytestring_string(bytestring.begin(), bytestring.end());
    opcua_value = UA_BYTESTRING_ALLOC(bytestring_string.c_str());
}

template<>
void set_opc_ua_value<UA_ByteString, opcua::types::ByteString>(
        UA_ByteString& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id)
{
    std::vector<uint8_t> bytestring;
    dd_variant.get_values(dd_variant_member_id, bytestring);
    std::string bytestring_string(bytestring.begin(), bytestring.end());
    opcua_value = UA_BYTESTRING_ALLOC(bytestring_string.c_str());
}

template<>
void set_opc_ua_value<UA_NodeId, opcua::types::NodeId>(
        UA_NodeId& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    LoanedDynamicData dd_nodeid = dd_variant.loan_value(dd_variant_member_name);
    dds_dynamic_data_to_opc_ua_node_id(opcua_value, dd_nodeid);
}

template<>
void set_opc_ua_value<UA_NodeId, opcua::types::NodeId>(
        UA_NodeId& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id)
{
    LoanedDynamicData dd_nodeid = dd_variant.loan_value(dd_variant_member_id);
    dds_dynamic_data_to_opc_ua_node_id(opcua_value, dd_nodeid);
}

template<>
void set_opc_ua_value<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        UA_ExpandedNodeId& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    LoanedDynamicData dd_expandednodeid =
            dd_variant.loan_value(dd_variant_member_name);
    dds_dynamic_data_to_opc_ua_expanded_node_id(opcua_value, dd_expandednodeid);
}

template<>
void set_opc_ua_value<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        UA_ExpandedNodeId& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id)
{
    LoanedDynamicData dd_expandednodeid =
            dd_variant.loan_value(dd_variant_member_id);
    dds_dynamic_data_to_opc_ua_expanded_node_id(opcua_value, dd_expandednodeid);
}

template<>
void set_opc_ua_value<UA_QualifiedName, opcua::types::QualifiedName>(
        UA_QualifiedName& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    LoanedDynamicData dd_qualifiedname =
            dd_variant.loan_value(dd_variant_member_name);
    opcua_value.namespaceIndex =
            dd_qualifiedname.get().value<uint16_t>("namespace_index");
    opcua_value.name = UA_STRING_ALLOC(
            dd_qualifiedname.get().value<std::string>("name").c_str());
}

template<>
void set_opc_ua_value<UA_QualifiedName, opcua::types::QualifiedName>(
        UA_QualifiedName& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id)
{
    LoanedDynamicData dd_qualifiedname =
            dd_variant.loan_value(dd_variant_member_id);
    opcua_value.namespaceIndex =
            dd_qualifiedname.get().value<uint16_t>("namespace_index");
    opcua_value.name = UA_STRING_ALLOC(
            dd_qualifiedname.get().value<std::string>("name").c_str());
}

template<>
void set_opc_ua_value<UA_LocalizedText, opcua::types::LocalizedText>(
        UA_LocalizedText& opcua_value,
        DynamicData& dd_variant,
        const std::string& dd_variant_member_name)
{
    LoanedDynamicData dd_localizedtext =
            dd_variant.loan_value(dd_variant_member_name);
    opcua_value.locale = UA_STRING_ALLOC(
            dd_localizedtext.get().value<std::string>("locale").c_str());
    opcua_value.text = UA_STRING_ALLOC(
            dd_localizedtext.get().value<std::string>("text").c_str());
}

template<>
void set_opc_ua_value<UA_LocalizedText, opcua::types::LocalizedText>(
        UA_LocalizedText& opcua_value,
        DynamicData& dd_variant,
        const uint32_t dd_variant_member_id)
{
    LoanedDynamicData dd_localizedtext =
            dd_variant.loan_value(dd_variant_member_id);
    opcua_value.locale = UA_STRING_ALLOC(
            dd_localizedtext.get().value<std::string>("locale").c_str());
    opcua_value.text = UA_STRING_ALLOC(
            dd_localizedtext.get().value<std::string>("text").c_str());
}

opcua::sdk::types::NodeId dds_dynamic_data_node_id_to_opc_ua_node_id(
        DynamicData& data)
{
    uint16_t namespace_index = data.value<uint16_t>("namespace_index");
    LoanedDynamicData identifier_type = data.loan_value("identifier_type");

    switch (static_cast<opcua::types::NodeIdentifierKind>(
            identifier_type.get().discriminator_value())) {
    case opcua::types::NodeIdentifierKind::NUMERIC_NODE_ID:
        return opcua::sdk::types::NodeId(
                namespace_index,
                identifier_type.get().value<uint32_t>("numeric_id"));
    case opcua::types::NodeIdentifierKind::STRING_NODE_ID:
    default:
        return opcua::sdk::types::NodeId(
                namespace_index,
                identifier_type.get().value<std::string>("string_id"));
    }
}

void dds_dynamic_data_variant_type_to_opc_ua_variant_scalar(
        opcua::sdk::types::Variant& variant,
        DynamicData& variant_value,
        const uint32_t variant_value_type)
{
    switch (static_cast<opcua::types::BuiltinTypeKind>(variant_value_type)) {
    case opcua::types::BuiltinTypeKind::BOOLEAN_TYPE: {
        set_variant_scalar<UA_Boolean, bool>(
                variant,
                variant_value,
                "bool_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::SBYTE_TYPE: {
        set_variant_scalar<UA_SByte, DDS_Octet>(
                variant,
                variant_value,
                "sbyte_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::BYTE_TYPE: {
        set_variant_scalar<UA_Byte, DDS_Octet>(
                variant,
                variant_value,
                "byte_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::INT16_TYPE: {
        set_variant_scalar<UA_Int16, int16_t>(
                variant,
                variant_value,
                "int16_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::UINT16_TYPE: {
        set_variant_scalar<UA_UInt16, uint16_t>(
                variant,
                variant_value,
                "uint16_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::INT32_TYPE: {
        set_variant_scalar<UA_Int32, int32_t>(
                variant,
                variant_value,
                "int32_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::UINT32_TYPE: {
        set_variant_scalar<UA_UInt32, uint32_t>(
                variant,
                variant_value,
                "uint32_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::INT64_TYPE: {
        set_variant_scalar<UA_Int64, rti::core::int64>(
                variant,
                variant_value,
                "int64_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::UINT64_TYPE: {
        set_variant_scalar<UA_UInt64, rti::core::uint64>(
                variant,
                variant_value,
                "uint64_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::FLOAT_TYPE: {
        set_variant_scalar<UA_Float, DDS_Float>(
                variant,
                variant_value,
                "float_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::DOUBLE_TYPE: {
        set_variant_scalar<UA_Double, DDS_Double>(
                variant,
                variant_value,
                "double_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::STRING_TYPE: {
        set_variant_scalar<UA_String, std::string>(
                variant,
                variant_value,
                "string_value[0]");
    } break;
    case rti::opcua::types::BuiltinTypeKind::DATETIME_TYPE: {
        set_variant_scalar<UA_Int64, opcua::types::DateTime>(
                variant,
                variant_value,
                "datetime_value[0]");
    } break;
    case rti::opcua::types::BuiltinTypeKind::GUID_TYPE: {
        set_variant_scalar<UA_Guid, opcua::types::Guid>(
                variant,
                variant_value,
                "guid_value[0]");
    } break;
    case rti::opcua::types::BuiltinTypeKind::BYTESTRING_TYPE: {
        set_variant_scalar<UA_ByteString, opcua::types::ByteString>(
                variant,
                variant_value,
                "bytestring_value[0]");
    } break;
    case rti::opcua::types::BuiltinTypeKind::XMLELEMENT_TYPE: {
        set_variant_scalar<UA_String, std::string>(
                variant,
                variant_value,
                "xmlelement_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::NODEID_TYPE: {
        set_variant_scalar<UA_NodeId, opcua::types::NodeId>(
                variant,
                variant_value,
                "nodeid_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::EXPANDEDNODEID_TYPE: {
        set_variant_scalar<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
                variant,
                variant_value,
                "expandednodeid_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::STATUSCODE_TYPE: {
        set_variant_scalar<UA_UInt32, uint32_t>(
                variant,
                variant_value,
                "statuscode_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::QUALIFIEDNAME_TYPE: {
        set_variant_scalar<UA_QualifiedName, opcua::types::QualifiedName>(
                variant,
                variant_value,
                "qualifiedname_value[0]");
    } break;
    case opcua::types::BuiltinTypeKind::LOCALIZEDTEXT_TYPE: {
        set_variant_scalar<UA_LocalizedText, opcua::types::LocalizedText>(
                variant,
                variant_value,
                "localizedtext_value[0]");
    } break;
    }
}

void dds_dynamic_data_variant_type_to_opc_ua_variant_array(
        opcua::sdk::types::Variant& variant,
        DynamicData& variant_value,
        const uint32_t variant_value_type)
{
    switch (static_cast<opcua::types::BuiltinTypeKind>(variant_value_type)) {
    case opcua::types::BuiltinTypeKind::BOOLEAN_TYPE: {
        set_variant_array<UA_Boolean, DDS_Boolean>(
                variant,
                variant_value,
                "bool_value");
    } break;
    case opcua::types::BuiltinTypeKind::SBYTE_TYPE: {
        set_variant_array<UA_SByte, DDS_Octet>(
                variant,
                variant_value,
                "sbyte_value");
    } break;
    case opcua::types::BuiltinTypeKind::BYTE_TYPE: {
        set_variant_array<UA_Byte, DDS_Octet>(
                variant,
                variant_value,
                "byte_value");
    } break;
    case opcua::types::BuiltinTypeKind::INT16_TYPE: {
        set_variant_array<UA_Int16, int16_t>(
                variant,
                variant_value,
                "int16_value");
    } break;
    case opcua::types::BuiltinTypeKind::UINT16_TYPE: {
        set_variant_array<UA_UInt16, uint16_t>(
                variant,
                variant_value,
                "uint16_value");
    } break;
    case opcua::types::BuiltinTypeKind::INT32_TYPE: {
        set_variant_array<UA_Int32, int32_t>(
                variant,
                variant_value,
                "int32_value");
    } break;
    case opcua::types::BuiltinTypeKind::UINT32_TYPE: {
        set_variant_array<UA_UInt32, uint32_t>(
                variant,
                variant_value,
                "uint32_value");
    } break;
    case opcua::types::BuiltinTypeKind::INT64_TYPE: {
        set_variant_array<UA_Int64, rti::core::int64>(
                variant,
                variant_value,
                "int64_value");
    } break;
    case opcua::types::BuiltinTypeKind::UINT64_TYPE: {
        set_variant_array<UA_UInt64, rti::core::uint64>(
                variant,
                variant_value,
                "uint64_value");
    } break;
    case opcua::types::BuiltinTypeKind::FLOAT_TYPE: {
        set_variant_array<UA_Float, DDS_Float>(
                variant,
                variant_value,
                "float_value");
    } break;
    case opcua::types::BuiltinTypeKind::DOUBLE_TYPE: {
        set_variant_array<UA_Double, DDS_Double>(
                variant,
                variant_value,
                "double_value");
    } break;
    case opcua::types::BuiltinTypeKind::STRING_TYPE: {
        set_variant_array<UA_String, std::string>(
                variant,
                variant_value,
                "string_value");
    } break;
    case opcua::types::BuiltinTypeKind::DATETIME_TYPE: {
        set_variant_array<UA_Int64, rti::core::int64>(
                variant,
                variant_value,
                "datetime_value");
    } break;
    case opcua::types::BuiltinTypeKind::GUID_TYPE: {
        set_variant_array<UA_Guid, opcua::types::Guid>(
                variant,
                variant_value,
                "guid_value");
    } break;
    case opcua::types::BuiltinTypeKind::BYTESTRING_TYPE: {
        set_variant_array<UA_ByteString, opcua::types::ByteString>(
                variant,
                variant_value,
                "bytestring_value");
    } break;
    case opcua::types::BuiltinTypeKind::XMLELEMENT_TYPE: {
        set_variant_array<UA_String, std::string>(
                variant,
                variant_value,
                "xmlelement_value");
    } break;
    case opcua::types::BuiltinTypeKind::NODEID_TYPE: {
        set_variant_array<UA_NodeId, opcua::types::NodeId>(
                variant,
                variant_value,
                "nodeid_value");
    } break;
    case opcua::types::BuiltinTypeKind::EXPANDEDNODEID_TYPE: {
        set_variant_array<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
                variant,
                variant_value,
                "expandednodeid_value");
    } break;
    case opcua::types::BuiltinTypeKind::STATUSCODE_TYPE: {
        set_variant_array<UA_UInt32, uint32_t>(
                variant,
                variant_value,
                "statuscode_value");
    } break;
    case opcua::types::BuiltinTypeKind::QUALIFIEDNAME_TYPE: {
        set_variant_array<UA_QualifiedName, opcua::types::QualifiedName>(
                variant,
                variant_value,
                "qualifiedname_value");
    } break;
    case opcua::types::BuiltinTypeKind::LOCALIZEDTEXT_TYPE: {
        set_variant_array<UA_LocalizedText, opcua::types::LocalizedText>(
                variant,
                variant_value,
                "localizedtext_value");
    } break;
    default:
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_ANY_FAILURE_s,
                "Unsuported type");
    }
}

void dds_dynamic_data_variant_type_to_opc_ua_variant(
        opcua::sdk::types::Variant& variant,
        const DynamicData& data)
{
    std::vector<uint32_t> array_dimensions =
            data.get_values<uint32_t>("array_dimensions");

    DynamicData& dd_variant = const_cast<DynamicData&>(data);
    LoanedDynamicData variant_value = dd_variant.loan_value("value");
    uint32_t variant_value_type = variant_value.get().discriminator_value();

    if (array_dimensions.size() == 0) {
        dds_dynamic_data_variant_type_to_opc_ua_variant_scalar(
                variant,
                variant_value,
                variant_value_type);
    } else {
        dds_dynamic_data_variant_type_to_opc_ua_variant_array(
                variant,
                variant_value,
                variant_value_type);
    }
}

void dds_dynamic_data_array_member_to_dds_dynamic_data_variant(
        DynamicData& variant,
        const DynamicData& data,
        const std::string& member_name)
{
    dds::core::xtypes::TypeKind kind =
            data.member_info(member_name).element_kind();
    std::vector<uint32_t> array_dimensions(
            1,
            data.member_info(member_name).element_count());
    variant.set_values("array_dimensions", array_dimensions);
    switch (kind.underlying()) {
    case dds::core::xtypes::TypeKind::BOOLEAN_TYPE: {
        std::vector<DDS_Boolean> values =
                data.get_values<DDS_Boolean>(member_name);
        DDS_ReturnCode_t retcode = DDS_DynamicData_set_boolean_array(
                &variant.native(),
                "value.bool_value",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                values.size(),
                &values[0]);
        check_dynamic_data_return_code(
                retcode,
                "Failed to set DDS_Boolean array");
    } break;
    case dds::core::xtypes::TypeKind::UINT_8_TYPE: {
        std::vector<DDS_Octet> values = data.get_values<DDS_Octet>(member_name);
        DDS_ReturnCode_t retcode = DDS_DynamicData_set_octet_array(
                &variant.native(),
                "value.byte_value",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                values.size(),
                &values[0]);
        check_dynamic_data_return_code(
                retcode,
                "Failed to set DDS_Octet array");
    } break;
    case dds::core::xtypes::TypeKind::INT_16_TYPE: {
        variant.set_values(
                "value.int16_value",
                data.get_values<int16_t>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::UINT_16_TYPE: {
        std::vector<uint16_t> values = data.get_values<uint16_t>(member_name);
        DDS_ReturnCode_t retcode = DDS_DynamicData_set_ushort_array(
                &variant.native(),
                "value.uint16_value",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                values.size(),
                &values[0]);
        check_dynamic_data_return_code(retcode, "Failed to set uint16_t array");
    } break;
    case dds::core::xtypes::TypeKind::INT_32_TYPE: {
        variant.set_values(
                "value.int32_value",
                data.get_values<int32_t>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::UINT_32_TYPE: {
        variant.set_values(
                "value.uint32_value",
                data.get_values<uint32_t>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::INT_64_TYPE: {
        variant.set_values(
                "value.int64_value",
                data.get_values<rti::core::int64>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::UINT_64_TYPE: {
        variant.set_values(
                "value.uint64_value",
                data.get_values<rti::core::uint64>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::FLOAT_32_TYPE: {
        variant.set_values(
                "value.float_value",
                data.get_values<float>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::FLOAT_64_TYPE: {
        variant.set_values(
                "value.double_value",
                data.get_values<double>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::CHAR_8_TYPE: {
        std::vector<char> values = data.get_values<char>(member_name);
        rti::core::xtypes::LoanedDynamicData string_array =
                variant.loan_value("value.string_value");
        for (int i = 0; i < values.size(); i++) {
            string_array.get().value<std::string>(
                    i + 1,
                    std::string(1, values[i]));
        }
    } break;
    case dds::core::xtypes::TypeKind::STRING_TYPE: {
        rti::core::xtypes::LoanedDynamicData values =
                const_cast<DynamicData&>(data).loan_value(member_name);
        rti::core::xtypes::LoanedDynamicData string_array =
                variant.loan_value("value.string_value");
        for (int i = 1; i < values.get().member_count(); i++) {
            string_array.get().value<std::string>(
                    i,
                    values.get().value<std::string>(i));
        }
    } break;
    // case dds::core::xtypes::TypeKind::ARRAY_TYPE: {
    //     dds::core::xtypes::TypeKind kind =
    //             data.member_info(member_name).element_kind();
    // } break;
    // case dds::core::xtypes::TypeKind::SEQUENCE_TYPE: {
    // } break;
    default:
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_ANY_FAILURE_s,
                "Unsuported type");
    }
}

void dds_dynamic_data_member_to_dds_dynamic_data_variant(
        DynamicData& variant,
        const DynamicData& data,
        const std::string& member_name)
{
    variant.clear_member("array_dimensions");

    dds::core::xtypes::TypeKind kind =
            data.member_info(member_name).member_kind();
    switch (kind.underlying()) {
    case dds::core::xtypes::TypeKind::BOOLEAN_TYPE: {
        variant.value<bool>(
                "value.bool_value[0]",
                data.value<bool>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::UINT_8_TYPE: {
        variant.value<DDS_Octet>(
                "value.byte_value[0]",
                data.value<DDS_Octet>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::INT_16_TYPE: {
        variant.value<int16_t>(
                "value.int16_value[0]",
                data.value<int16_t>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::UINT_16_TYPE: {
        DDS_ReturnCode_t retcode = DDS_DynamicData_set_ushort(
                &variant.native(),
                "value.uint16_value[0]",
                DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
                data.value<uint16_t>(member_name));
        check_dynamic_data_return_code(retcode, "Failed to get uint16_t value");
    } break;
    case dds::core::xtypes::TypeKind::INT_32_TYPE: {
        variant.value<int32_t>(
                "value.int32_value[0]",
                data.value<int32_t>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::UINT_32_TYPE: {
        variant.value<uint32_t>(
                "value.uint32_value[0]",
                data.value<uint32_t>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::INT_64_TYPE: {
        variant.value<rti::core::int64>(
                "value.int64_value[0]",
                data.value<rti::core::int64>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::UINT_64_TYPE: {
        variant.value<rti::core::uint64>(
                "value.uint64_value[0]",
                data.value<rti::core::uint64>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::FLOAT_32_TYPE: {
        variant.value<float>(
                "value.float_value[0]",
                data.value<float>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::FLOAT_64_TYPE: {
        variant.value<double>(
                "value.double_value[0]",
                data.value<double>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::CHAR_8_TYPE: {
        variant.value<std::string>(
                "value.string_value[0]",
                std::string(1, data.value<DDS_Char>(member_name)));
    } break;
    case dds::core::xtypes::TypeKind::STRING_TYPE: {
        variant.value<std::string>(
                "value.string_value[0]",
                data.value<std::string>(member_name));
    } break;
    case dds::core::xtypes::TypeKind::ARRAY_TYPE:
    case dds::core::xtypes::TypeKind::SEQUENCE_TYPE: {
        dds_dynamic_data_array_member_to_dds_dynamic_data_variant(
                variant,
                data,
                member_name);
    } break;
    default:
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_ANY_FAILURE_s,
                "Unsuported type");
    }
}

}}}  // namespace rti::ddsopcua::conversion
