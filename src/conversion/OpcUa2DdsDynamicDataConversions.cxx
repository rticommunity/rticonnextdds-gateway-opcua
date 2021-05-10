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

#include "conversion/OpcUaBuiltinDataTypes.hpp"
#include "log/LogMsg.hpp"

#include "OpcUa2DdsDynamicDataConversions.hpp"
#include "DdsDynamicDataUtils.hpp"

namespace rti { namespace ddsopcua { namespace conversion {

void opc_ua_date_time_to_dds_time_t(
        DDS_Time_t& dds_time,
        const UA_DateTime& opc_ua_date)
{
    // Get Unix timestamp in microseconds
    int64_t microsec =
            (opc_ua_date - UA_DATETIME_UNIX_EPOCH) / UA_DATETIME_USEC;

    // Transform unix time in microseconds to DDS_Time_t -- this won't work
    // for negative UA_DateTime. We don't expect negative timestamps which is
    // what we use this conversion for.
    const uint64_t DDS_LONG_MAX = 0x7FFFFFFFLL;
    if (microsec <= DDS_LONG_MAX * 1000000 + 999999) {
        dds_time.sec =
                static_cast<int32_t>(microsec / rti::core::microsec_per_sec);
        dds_time.nanosec = static_cast<uint32_t>(
                (microsec % rti::core::microsec_per_sec)
                * rti::core::nanosec_per_microsec);
    } else {
        dds_time = DDS_TIME_MAX;
    }
}

void opc_ua_node_id_to_dds_dynamic_data(
        DynamicData& dd_nodeid,
        const UA_NodeId* opcua_nodeid)
{
    dd_nodeid.value(
            "namespace_index",
            static_cast<uint16_t>(opcua_nodeid->namespaceIndex));

    rti::core::xtypes::LoanedDynamicData identifier_type =
            dd_nodeid.loan_value("identifier_type");
    switch (opcua_nodeid->identifierType) {
    case UA_NODEIDTYPE_NUMERIC: {
        set_dynamic_data_member<UA_UInt32, uint32_t>(
                identifier_type.get(),
                &opcua_nodeid->identifier.numeric,
                "numeric_id");
    } break;
    case UA_NODEIDTYPE_STRING: {
        set_dynamic_data_member<UA_String, std::string>(
                identifier_type.get(),
                &opcua_nodeid->identifier.string,
                "string_id");
    } break;
    case UA_NODEIDTYPE_BYTESTRING: {
        set_dynamic_data_member<UA_ByteString, types::ByteString>(
                identifier_type.get(),
                &opcua_nodeid->identifier.byteString,
                "opaque_id");
    } break;
    case UA_NODEIDTYPE_GUID: {
        set_dynamic_data_member<UA_Guid, types::Guid>(
                identifier_type.get(),
                &opcua_nodeid->identifier.guid,
                "guid_id");
    } break;
    }
}

void opc_ua_guid_to_dds_dynamic_data(
        DynamicData& dd_guid,
        const UA_Guid* opcua_guid)
{
    set_dynamic_data_member<UA_UInt32, uint32_t>(
            dd_guid,
            &opcua_guid->data1,
            "data1");
    set_dynamic_data_member<UA_UInt16, uint16_t>(
            dd_guid,
            &opcua_guid->data2,
            "data2");
    set_dynamic_data_member<UA_UInt16, uint16_t>(
            dd_guid,
            &opcua_guid->data3,
            "data3");

    std::vector<uint8_t> data4;
    data4.assign(
            const_cast<uint8_t*>(opcua_guid->data4),
            const_cast<uint8_t*>(opcua_guid->data4) + 8);
    dd_guid.set_values("data4", data4);
}

void opc_ua_expanded_node_id_to_dds_dynamic_data(
        DynamicData& dd_expanded_nodeid,
        const UA_ExpandedNodeId* opcua_expandednodeid)
{
    set_dynamic_data_member<UA_String, std::string>(
            dd_expanded_nodeid,
            &opcua_expandednodeid->namespaceUri,
            "namespace_uri");
    set_dynamic_data_member<UA_UInt32, uint32_t>(
            dd_expanded_nodeid,
            &opcua_expandednodeid->serverIndex,
            "server_index");
    opc_ua_node_id_to_dds_dynamic_data(
            dd_expanded_nodeid,
            &opcua_expandednodeid->nodeId);
}

inline void opc_ua_bytestring_to_uint8_vector(
        std::vector<uint8_t>& v,
        const UA_ByteString* opcua_data)
{
    v.assign(
            static_cast<uint8_t*>(opcua_data->data),
            static_cast<uint8_t*>(opcua_data->data) + opcua_data->length);
}

template<>
void set_dynamic_data_member<UA_Boolean, DDS_Boolean>(
        DynamicData& dd_value,
        const UA_Boolean* opcua_value,
        const std::string& dd_member_name)
{
    DDS_ReturnCode_t retcode = DDS_DynamicData_set_boolean(
            &dd_value.native(),
            dd_member_name.c_str(),
            DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
            static_cast<DDS_Boolean>(*opcua_value));
    check_dynamic_data_return_code(retcode, "Failed to get boolean value");
}

template<>
void set_dynamic_data_member<UA_Boolean, DDS_Boolean>(
        DynamicData& dd_value,
        const UA_Boolean* opcua_value,
        const uint32_t dd_member_id)
{
    DDS_ReturnCode_t retcode = DDS_DynamicData_set_boolean(
            &dd_value.native(),
            dds::core::null,
            dd_member_id,
            static_cast<DDS_Boolean>(*opcua_value));
    check_dynamic_data_return_code(retcode, "Failed to get boolean value");
}


template<>
void set_dynamic_data_member<UA_UInt16, uint16_t>(
        DynamicData& dd_value,
        const UA_UInt16* opcua_value,
        const std::string& dd_member_name)
{
    DDS_ReturnCode_t retcode = DDS_DynamicData_set_ushort(
            &dd_value.native(),
            dd_member_name.c_str(),
            DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
            static_cast<uint16_t>(*opcua_value));
    check_dynamic_data_return_code(retcode, "Failed to get uint16_t value");
}

template<>
void set_dynamic_data_member<UA_UInt16, uint16_t>(
        DynamicData& dd_value,
        const UA_UInt16* opcua_value,
        const uint32_t dd_member_id)
{
    DDS_ReturnCode_t retcode = DDS_DynamicData_set_ushort(
            &dd_value.native(),
            dds::core::null,
            dd_member_id,
            static_cast<uint16_t>(*opcua_value));
    check_dynamic_data_return_code(retcode, "Failed to get uint16_t value");
}

template<>
inline void set_dynamic_data_member<UA_String, std::string>(
        DynamicData& dds_data,
        const UA_String* opcua_data,
        const std::string& dds_member_name)
{
    dds_data.value(
            dds_member_name,
            std::string(
                    reinterpret_cast<const char*>(opcua_data->data),
                    opcua_data->length));
}

template<>
inline void set_dynamic_data_member<UA_String, std::string>(
        DynamicData& dds_data,
        const UA_String* opcua_data,
        const uint32_t dds_member_id)
{
    dds_data.value(
            dds_member_id,
            std::string(
                    reinterpret_cast<const char*>(opcua_data->data),
                    opcua_data->length));
}

template<>
inline void set_dynamic_data_member<UA_ByteString, opcua::types::ByteString>(
        DynamicData& dd_bytestring,
        const UA_ByteString* opcua_data,
        const std::string& dds_member_name)
{
    std::vector<uint8_t> dds_vector;
    opc_ua_bytestring_to_uint8_vector(dds_vector, opcua_data);

    // To workaround CORE-8061 we must use the DynamicData C API to set
    // ByteStrings within unions. Otherwise, we would simply use something
    // like dds_data.set_values(dds_member_name, data);
    DDS_ReturnCode_t retcode = DDS_DynamicData_set_octet_array(
            &dd_bytestring.native(),
            dds_member_name.c_str(),
            DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED,
            static_cast<DDS_UnsignedLong>(opcua_data->length),
            static_cast<uint8_t*>(opcua_data->data));
    check_dynamic_data_return_code(retcode, "Failed to get ByteString value");
}

template<>
inline void set_dynamic_data_member<UA_ByteString, opcua::types::ByteString>(
        DynamicData& dd_bytestring,
        const UA_ByteString* opcua_data,
        const uint32_t dds_member_id)
{
    std::vector<uint8_t> dds_vector;
    opc_ua_bytestring_to_uint8_vector(dds_vector, opcua_data);

    // To workaround CORE-8061 we must use the DynamicData C API to set
    // ByteStrings within unions. Otherwise, we would simply use something
    // like dds_data.set_values(dds_member_name, data);
    DDS_DynamicData_set_octet_array(
            &dd_bytestring.native(),
            dds::core::null,
            dds_member_id,
            static_cast<DDS_UnsignedLong>(opcua_data->length),
            static_cast<uint8_t*>(opcua_data->data));
}

template<>
inline void set_dynamic_data_member<UA_Guid, opcua::types::Guid>(
        DynamicData& dds_data,
        const UA_Guid* opcua_data,
        const std::string& dds_member_name)
{
    LoanedDynamicData guid_member = dds_data.loan_value(dds_member_name);
    opc_ua_guid_to_dds_dynamic_data(guid_member.get(), opcua_data);
}

template<>
inline void set_dynamic_data_member<UA_Guid, opcua::types::Guid>(
        DynamicData& dds_data,
        const UA_Guid* opcua_data,
        const uint32_t dds_member_id)
{
    LoanedDynamicData guid_member = dds_data.loan_value(dds_member_id);
    opc_ua_guid_to_dds_dynamic_data(guid_member.get(), opcua_data);
}

template<>
void set_dynamic_data_member<UA_NodeId, opcua::types::NodeId>(
        DynamicData& dds_data,
        const UA_NodeId* opcua_data,
        const std::string& dds_member_name)
{
    LoanedDynamicData node_id = dds_data.loan_value(dds_member_name);
    opc_ua_node_id_to_dds_dynamic_data(node_id.get(), opcua_data);
}

template<>
void set_dynamic_data_member<UA_NodeId, opcua::types::NodeId>(
        DynamicData& dds_data,
        const UA_NodeId* opcua_data,
        const uint32_t dds_member_id)
{
    LoanedDynamicData node_id = dds_data.loan_value(dds_member_id);
    opc_ua_node_id_to_dds_dynamic_data(node_id.get(), opcua_data);
}

template<>
void set_dynamic_data_member<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        DynamicData& dds_data,
        const UA_ExpandedNodeId* opcua_data,
        const std::string& dds_member_name)
{
    LoanedDynamicData expanded_node_id = dds_data.loan_value(dds_member_name);
    opc_ua_expanded_node_id_to_dds_dynamic_data(
            expanded_node_id.get(),
            opcua_data);
}

template<>
void set_dynamic_data_member<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        DynamicData& dds_data,
        const UA_ExpandedNodeId* opcua_data,
        const uint32_t dds_member_id)
{
    LoanedDynamicData expanded_node_id = dds_data.loan_value(dds_member_id);
    opc_ua_expanded_node_id_to_dds_dynamic_data(
            expanded_node_id.get(),
            opcua_data);
}

template<>
void set_dynamic_data_member<UA_QualifiedName, opcua::types::QualifiedName>(
        DynamicData& dds_data,
        const UA_QualifiedName* opcua_data,
        const std::string& dds_member_name)
{
    LoanedDynamicData qualified_name = dds_data.loan_value(dds_member_name);
    set_dynamic_data_member<UA_UInt16, uint16_t>(
            qualified_name.get(),
            &opcua_data->namespaceIndex,
            "namespace_index");
    set_dynamic_data_member<UA_String, std::string>(
            qualified_name.get(),
            &opcua_data->name,
            "name");
}

template<>
void set_dynamic_data_member<UA_QualifiedName, opcua::types::QualifiedName>(
        DynamicData& dds_data,
        const UA_QualifiedName* opcua_data,
        const uint32_t dds_member_id)
{
    LoanedDynamicData qualified_name = dds_data.loan_value(dds_member_id);
    set_dynamic_data_member<UA_UInt16, uint16_t>(
            qualified_name.get(),
            &opcua_data->namespaceIndex,
            "namespace_index");
    set_dynamic_data_member<UA_String, std::string>(
            qualified_name.get(),
            &opcua_data->name,
            "name");
}

template<>
void set_dynamic_data_member<UA_LocalizedText, opcua::types::LocalizedText>(
        DynamicData& dds_data,
        const UA_LocalizedText* opcua_data,
        const std::string& dds_member_name)
{
    LoanedDynamicData localized_text = dds_data.loan_value(dds_member_name);
    set_dynamic_data_member<UA_String, std::string>(
            localized_text.get(),
            &opcua_data->text,
            "text");
    set_dynamic_data_member<UA_String, std::string>(
            localized_text.get(),
            &opcua_data->locale,
            "locale");
}

template<>
void set_dynamic_data_member<UA_LocalizedText, opcua::types::LocalizedText>(
        DynamicData& dds_data,
        const UA_LocalizedText* opcua_data,
        const uint32_t dds_member_id)
{
    LoanedDynamicData localized_text = dds_data.loan_value(dds_member_id);
    set_dynamic_data_member<UA_String, std::string>(
            localized_text.get(),
            &opcua_data->text,
            "text");
    set_dynamic_data_member<UA_String, std::string>(
            localized_text.get(),
            &opcua_data->locale,
            "locale");
}

template<>
void set_dynamic_data_member<UA_ExtensionObject, types::ExtensionObject>(
        DynamicData& dds_data,
        const UA_ExtensionObject* opcua_data,
        const std::string& dds_member_name)
{
    RTI_THROW_GATEWAY_EXCEPTION(&DDSOPCUA_LOG_ANY_FAILURE_s, "Unsuported type");
}

template<>
void set_dynamic_data_member<UA_ExtensionObject, types::ExtensionObject>(
        DynamicData& dds_data,
        const UA_ExtensionObject* opcua_data,
        const uint32_t dds_member_id)
{
    RTI_THROW_GATEWAY_EXCEPTION(&DDSOPCUA_LOG_ANY_FAILURE_s, "Unsuported type");
}

std::function<void(DynamicData&, const UA_Variant&, const std::string&)>
opc_ua_scalar_variant_to_dds_dynamic_data_fnc(
        const sdk::types::BuiltinTypeKind& opcua_type_kind)
{
    switch (opcua_type_kind) {
    case sdk::types::BuiltinTypeKind::BOOL_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Boolean,
                        DDS_Boolean>);
    case sdk::types::BuiltinTypeKind::SBYTE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_SByte,
                        DDS_Octet>);
    case sdk::types::BuiltinTypeKind::BYTE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Byte,
                        DDS_Octet>);
    case sdk::types::BuiltinTypeKind::INT16_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Int16,
                        int16_t>);
    case sdk::types::BuiltinTypeKind::UINT16_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_UInt16,
                        uint16_t>);
    case sdk::types::BuiltinTypeKind::INT32_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Int32,
                        int32_t>);
    case sdk::types::BuiltinTypeKind::UINT32_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_UInt32,
                        uint32_t>);
    case sdk::types::BuiltinTypeKind::INT64_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Int64,
                        rti::core::int64>);
    case sdk::types::BuiltinTypeKind::UINT64_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_UInt64,
                        rti::core::uint64>);
    case sdk::types::BuiltinTypeKind::FLOAT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Float,
                        DDS_Float>);
    case sdk::types::BuiltinTypeKind::DOUBLE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Double,
                        DDS_Double>);
    case sdk::types::BuiltinTypeKind::STRING_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_String,
                        std::string>);
    case sdk::types::BuiltinTypeKind::DATETIME_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_DateTime,
                        opcua::types::DateTime>);
    case sdk::types::BuiltinTypeKind::GUID_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_Guid,
                        opcua::types::Guid>);
    case sdk::types::BuiltinTypeKind::BYTESTRING_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_ByteString,
                        opcua::types::ByteString>);
    case sdk::types::BuiltinTypeKind::XMLELEMENT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_String,
                        std::string>);
    case sdk::types::BuiltinTypeKind::NODEID_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_NodeId,
                        opcua::types::NodeId>);
    case sdk::types::BuiltinTypeKind::EXPANDEDNODEID_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_ExpandedNodeId,
                        opcua::types::ExpandedNodeId>);
    case sdk::types::BuiltinTypeKind::STATUSCODE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_StatusCode,
                        opcua::types::StatusCode>);
    case sdk::types::BuiltinTypeKind::QUALIFIEDNAME_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_QualifiedName,
                        opcua::types::QualifiedName>);
    case sdk::types::BuiltinTypeKind::LOCALIZEDTEXT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_LocalizedText,
                        opcua::types::LocalizedText>);
    case sdk::types::BuiltinTypeKind::EXTENSIONOBJECT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type<
                        UA_ExtensionObject,
                        opcua::types::ExtensionObject>);
    default:
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_ANY_FAILURE_s,
                "Unsuported type");
    }
}

std::function<void(DynamicData&, const UA_Variant&, const std::string&)>
opc_ua_array_variant_to_dds_dynamic_data_fnc(
        const sdk::types::BuiltinTypeKind& opcua_type_kind)
{
    switch (opcua_type_kind) {
    case sdk::types::BuiltinTypeKind::BOOL_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Boolean,
                        DDS_Boolean>);
    case sdk::types::BuiltinTypeKind::SBYTE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_SByte,
                        DDS_Octet>);
    case sdk::types::BuiltinTypeKind::BYTE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Byte,
                        DDS_Octet>);
    case sdk::types::BuiltinTypeKind::INT16_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Int16,
                        int16_t>);
    case sdk::types::BuiltinTypeKind::UINT16_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_UInt16,
                        uint16_t>);
    case sdk::types::BuiltinTypeKind::INT32_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Int32,
                        int32_t>);
    case sdk::types::BuiltinTypeKind::UINT32_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_UInt32,
                        uint32_t>);
    case sdk::types::BuiltinTypeKind::INT64_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Int64,
                        rti::core::int64>);
    case sdk::types::BuiltinTypeKind::UINT64_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_UInt64,
                        rti::core::uint64>);
    case sdk::types::BuiltinTypeKind::FLOAT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Float,
                        DDS_Float>);
    case sdk::types::BuiltinTypeKind::DOUBLE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Double,
                        DDS_Double>);
    case sdk::types::BuiltinTypeKind::STRING_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_String,
                        std::string>);
    case sdk::types::BuiltinTypeKind::DATETIME_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_DateTime,
                        rti::core::int64>);
    case sdk::types::BuiltinTypeKind::GUID_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_Guid,
                        opcua::types::Guid>);
    case sdk::types::BuiltinTypeKind::BYTESTRING_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_ByteString,
                        opcua::types::ByteString>);
    case sdk::types::BuiltinTypeKind::XMLELEMENT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_String,
                        std::string>);
    case sdk::types::BuiltinTypeKind::NODEID_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_NodeId,
                        opcua::types::NodeId>);
    case sdk::types::BuiltinTypeKind::EXPANDEDNODEID_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_ExpandedNodeId,
                        opcua::types::ExpandedNodeId>);
    case sdk::types::BuiltinTypeKind::STATUSCODE_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_StatusCode,
                        opcua::types::StatusCode>);
    case sdk::types::BuiltinTypeKind::QUALIFIEDNAME_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_QualifiedName,
                        opcua::types::QualifiedName>);
    case sdk::types::BuiltinTypeKind::LOCALIZEDTEXT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_LocalizedText,
                        opcua::types::LocalizedText>);
    case sdk::types::BuiltinTypeKind::EXTENSIONOBJECT_TYPE:
        return static_cast<
                void (*)(DynamicData&, const UA_Variant&, const std::string&)>(
                opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type<
                        UA_ExtensionObject,
                        opcua::types::ExtensionObject>);
    default:
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_ANY_FAILURE_s,
                "Unsuported type");
    }
}


void opc_ua_array_dimensions_to_dds_dynamic_data_array_dimensions(
        DynamicData& dynamicdata_variant,
        const opcua::sdk::types::Variant& opcua_variant)
{
    if (UA_Variant_isScalar(opcua_variant.get_const_ref())
        || opcua_variant.get_const_ref()->type == 0) {
        dynamicdata_variant.clear_member("array_dimensions");
    } else {
        std::vector<uint32_t> array_dimensions;
        if (opcua_variant.get_const_ref()->arrayDimensionsSize == 0) {
            /*
             * If arrayDimensionsSize is zero we assume the Variant holds an
             * unbounded one-dimensional array, which is represented with
             * an array_dimensions array of one element equal to zero.
             */
            array_dimensions.push_back(0);
        } else {
            // Otherwise we copy the arrayDimensions of the Variant.
            array_dimensions.assign(
                    opcua_variant.get_const_ref()->arrayDimensions,
                    opcua_variant.get_const_ref()->arrayDimensions
                            + opcua_variant.get_const_ref()
                                      ->arrayDimensionsSize);
        }

        dynamicdata_variant.set_values("array_dimensions", array_dimensions);
    }
}

void opc_ua_variant_value_to_dds_dynamic_data_variant_value(
        DynamicData& dynamicdata_variant,
        const opcua::sdk::types::Variant& opcua_variant)
{
    LoanedDynamicData variant_value = dynamicdata_variant.loan_value("value");

    switch (opcua_variant.get_type_kind()) {
    case opcua::sdk::types::BuiltinTypeKind::BOOL_TYPE: {
        variant_value.get().loan_value("bool_value");
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Boolean,
                DDS_Boolean>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "bool_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::SBYTE_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_SByte,
                DDS_Octet>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "sbyte_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::BYTE_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Byte,
                DDS_Octet>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "byte_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::INT16_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Int16,
                int16_t>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "int16_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::UINT16_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_UInt16,
                uint16_t>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "uint16_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::INT32_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Int32,
                int32_t>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "int32_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::UINT32_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_UInt32,
                uint32_t>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "uint32_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::INT64_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Int64,
                rti::core::int64>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "int64_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::UINT64_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_UInt64,
                rti::core::uint64>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "uint64_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::FLOAT_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Float,
                DDS_Float>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "float_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::DOUBLE_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Double,
                DDS_Double>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "double_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::STRING_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_String,
                std::string>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "string_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::DATETIME_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_DateTime,
                rti::core::int64>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "datetime_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::GUID_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_Guid,
                opcua::types::Guid>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "guid_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::BYTESTRING_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_ByteString,
                opcua::types::ByteString>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "bytestring_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::XMLELEMENT_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_String,
                std::string>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "xmlelement_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::NODEID_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_NodeId,
                opcua::types::NodeId>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "nodeid_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::EXPANDEDNODEID_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_ExpandedNodeId,
                opcua::types::ExpandedNodeId>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "expandednodeid_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::STATUSCODE_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_StatusCode,
                opcua::types::StatusCode>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "statuscode_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::QUALIFIEDNAME_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_QualifiedName,
                opcua::types::QualifiedName>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "qualifiedname_value");
    } break;
    case opcua::sdk::types::BuiltinTypeKind::LOCALIZEDTEXT_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_LocalizedText,
                opcua::types::LocalizedText>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "localizedtext_value");
    } break;
    case sdk::types::BuiltinTypeKind::EXTENSIONOBJECT_TYPE: {
        opc_ua_variant_value_to_dds_dynamic_data_variant_value<
                UA_ExtensionObject,
                opcua::types::ExtensionObject>(
                variant_value.get(),
                *opcua_variant.get_const_ref(),
                "extensionobject_value");
    } break;
    }
}

void opc_ua_variant_to_dds_dynamic_data_variant_type(
        DynamicData& dynamicdata_variant,
        const opcua::sdk::types::Variant& opcua_variant)
{
    opc_ua_array_dimensions_to_dds_dynamic_data_array_dimensions(
            dynamicdata_variant,
            opcua_variant);
    opc_ua_variant_value_to_dds_dynamic_data_variant_value(
            dynamicdata_variant,
            opcua_variant);
}


}}}  // namespace rti::ddsopcua::conversion