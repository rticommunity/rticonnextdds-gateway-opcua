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

#ifndef RTI_CONVERSIONS_OPCUA2DDSDYNAMICDATACONVERSIONS_HPP_
#define RTI_CONVERSIONS_OPCUA2DDSDYNAMICDATACONVERSIONS_HPP_

#include <open62541/types.h>

#include <type_traits>

#include <dds/core/Time.hpp>
#include <dds/core/types.hpp>
#include <dds/core/xtypes/DynamicData.hpp>
#include <rti/core/Exception.hpp>

#include "conversion/OpcUaBuiltinDataTypes.hpp"
#include "opcUaSdk/OpcUaSdkDataTypes.hpp"

namespace rti { namespace ddsopcua { namespace conversion {

using namespace dds::core::xtypes;
using namespace rti::core::xtypes;
using namespace rti::opcua;

/**
 * @brief Set a dynamic data member object with an OPC UA value.
 *
 * @tparam OPCUA_TYPE OPC UA SDK Native type to be converted (e.g., UA_Boolean).
 * @tparam DDS_TYPE DDS SDK native type to be converted (e.g., DDS_Boolean).
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<typename OPCUA_TYPE, typename DDS_TYPE>
void set_dynamic_data_member(
        DynamicData& dd_value,
        const OPCUA_TYPE* opcua_value,
        const std::string& dd_member_name)
{
    dd_value.value(dd_member_name, static_cast<DDS_TYPE>(*opcua_value));
}

/**
 * @brief Set a dynamic data member object with an OPC UA value.
 *
 * @tparam OPCUA_TYPE OPC UA SDK Native type to be converted (e.g., UA_Boolean).
 * @tparam DDS_TYPE DDS SDK native type to be converted (e.g., DDS_Boolean).
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<typename OPCUA_TYPE, typename DDS_TYPE>
void set_dynamic_data_member(
        DynamicData& dd_value,
        const OPCUA_TYPE* opcua_value,
        const uint32_t dd_member_id)
{
    dd_value.value(dd_member_id, static_cast<DDS_TYPE>(*opcua_value));
}

/**
 * @brief Set the dynamic data member of type DDS Boolean with the value of
 * a UA Boolean
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_Boolean, DDS_Boolean>(
        DynamicData& dd_value,
        const UA_Boolean* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type DDS Boolean with the value of
 * a UA Boolean
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_Boolean, DDS_Boolean>(
        DynamicData& dd_value,
        const UA_Boolean* opcua_value,
        const uint32_t dd_member_id);

/**
 * @brief Set the dynamic data member of type uint16_t with the value
 * of a UA_UInt16.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_UInt16, uint16_t>(
        DynamicData& dd_value,
        const UA_UInt16* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type uint16_t with the value
 * of a UA_UInt16.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_UInt16, uint16_t>(
        DynamicData& dd_value,
        const UA_UInt16* opcua_value,
        const uint32_t dd_member_id);

/**
 * @brief Set the dynamic data member of type std::string with the value
 * of a UA_String.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_String, std::string>(
        DynamicData& dd_value,
        const UA_String* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type std::string with the value
 * of a UA_String.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_String, std::string>(
        DynamicData& dd_value,
        const UA_String* opcua_value,
        const uint32_t dds_member_id);

/**
 * @brief Set the dynamic data member of type DDS_Char with the value
 * of a UA_String (always teh first character).
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_String, DDS_Char>(
        DynamicData& dd_value,
        const UA_String* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type DDS_Char with the value
 * of a UA_String (always the first character).
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_String, DDS_Char>(
        DynamicData& dd_value,
        const UA_String* opcua_value,
        const uint32_t dds_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::Guid with the
 * value of a UA_Guid.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_Guid, opcua::types::Guid>(
        DynamicData& dd_value,
        const UA_Guid* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::Guid with the
 * value of a UA_Guid.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_Guid, opcua::types::Guid>(
        DynamicData& dd_value,
        const UA_Guid* opcua_value,
        const uint32_t dds_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::ByteString
 * with the value of a UA_ByteString.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_ByteString, opcua::types::ByteString>(
        DynamicData& dd_value,
        const UA_ByteString* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::ByteString
 * with the value of a UA_ByteString.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_ByteString, opcua::types::ByteString>(
        DynamicData& dd_value,
        const UA_ByteString* opcua_value,
        const uint32_t dds_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::XmlElement
 * with the value of a UA_XmlElement.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_XmlElement, opcua::types::XmlElement>(
        DynamicData& dd_value,
        const UA_XmlElement* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::XmlElement
 * with the value of a UA_XmlElement.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_XmlElement, opcua::types::XmlElement>(
        DynamicData& dd_value,
        const UA_XmlElement* opcua_value,
        const uint32_t dds_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::NodeId
 * with the value of a UA_NodeId.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_NodeId, opcua::types::NodeId>(
        DynamicData& dd_value,
        const UA_NodeId* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::NodeId
 * with the value of a UA_NodeId.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_NodeId, opcua::types::NodeId>(
        DynamicData& dd_value,
        const UA_NodeId* opcua_value,
        const uint32_t dd_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::ExpandedNodeId
 * with the value of a UA_ExpandedNodeId.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        DynamicData& dd_value,
        const UA_ExpandedNodeId* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::ExpandedNodeId
 * with the value of a UA_ExpandedNodeId.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_ExpandedNodeId, opcua::types::ExpandedNodeId>(
        DynamicData& dd_value,
        const UA_ExpandedNodeId* opcua_value,
        const uint32_t dd_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::QualifiedName
 * with the value of a UA_QualifiedName.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_QualifiedName, opcua::types::QualifiedName>(
        DynamicData& dd_value,
        const UA_QualifiedName* opcua_value,
        const std::string& dd_member_name);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::QualifiedName
 * with the value of a UA_QualifiedName.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_QualifiedName, opcua::types::QualifiedName>(
        DynamicData& dd_value,
        const UA_QualifiedName* opcua_value,
        const uint32_t dd_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::LocalizedText
 * with the value of a UA_LocalizedText.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_LocalizedText, opcua::types::LocalizedText>(
        DynamicData& dd_value,
        const UA_LocalizedText* opcua_value,
        const std::string& dds_member_name);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::LocalizedText
 * with the value of a UA_LocalizedText.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_LocalizedText, opcua::types::LocalizedText>(
        DynamicData& dd_value,
        const UA_LocalizedText* opcua_value,
        const std::string& dd_member_id);

/**
 * @brief Set the dynamic data member of type rti::opcua::types::ExtensionObject
 * with the value of a UA_ExtensionObject.
 *
 * This method is currently unsupported, so its invocation will result in an
 * rti::ddsopcua::GatewayException.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_name Name of the DDS member to be set within the
 * DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_ExtensionObject, types::ExtensionObject>(
        DynamicData& dd_value,
        const UA_ExtensionObject* opcua_value,
        const std::string& dd_member_name);


/**
 * @brief Set the dynamic data member of type rti::opcua::types::ExtensionObject
 * with the value of a UA_ExtensionObject.
 *
 * This method is currently unsupported, so its invocation will result in an
 * rti::ddsopcua::GatewayException.
 *
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source value.
 * @param dd_member_id DDS member ID to be set within the DynamicData object.
 */
template<>
void set_dynamic_data_member<UA_ExtensionObject, types::ExtensionObject>(
        DynamicData& dd_value,
        const UA_ExtensionObject* opcua_value,
        const uint32_t dd_member_id);

/**
 * @brief Set a member of a DynamicData object with the value of an OPC UA
 * Variant containing a scalar value.
 *
 * @tparam OPCUA_TYPE OPC UA type selected in the UA_Variant.
 * @tparam DDS_TYPE DDS type associated with the DynamicData member to be set.
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source variant.
 * @param dd_member_name DDS member to be set within the DynamicData object.
 */
template<typename OPCUA_TYPE, typename DDS_TYPE>
void opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type(
        DynamicData& dd_value,
        const UA_Variant& opcua_value,
        const std::string& dd_member_name)
{
    OPCUA_TYPE* opcua_data = static_cast<OPCUA_TYPE*>(opcua_value.data);
    set_dynamic_data_member<OPCUA_TYPE, DDS_TYPE>(
            dd_value,
            opcua_data,
            dd_member_name);
}

/**
 * @brief Set a member of a DynamicData object with the value of an OPC UA
 * Variant containing a scalar value.
 *
 * @tparam OPCUA_TYPE OPC UA type selected in the UA_Variant.
 * @tparam DDS_TYPE DDS type associated with the DynamicData member to be set.
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source variant.
 * @param dd_member_id ID of the member to be set within the DynamicData
 * object.
 */
template<typename OPCUA_TYPE, typename DDS_TYPE>
void opc_ua_variant_scalar_to_dds_dynamic_data_scalar_type(
        DynamicData& dd_value,
        const UA_Variant& opcua_value,
        const uint32_t dd_member_id)
{
    OPCUA_TYPE* opcua_data = static_cast<OPCUA_TYPE*>(opcua_value.data);
    set_dynamic_data_member<OPCUA_TYPE, DDS_TYPE>(
            dd_value,
            opcua_data,
            dd_member_id);
}

/**
 * @brief Function wrapper that provides access to the appropriate function to
 * set a DDS DynamicData member with the scalar value stored in an OPC UA
 * Variant.
 *
 * This method is useful in static functions, such as on_data_available
 * callbacks, that need to set the value of a DynamicData member without
 * much context of the value to be transformed.
 *
 * @param opcua_type_kind Type kind of the value held in the OPC UA Variant.
 * @param dds_type_kind Type kind of the value held in the DDS DynamicData
 * member.
 * @return std::function<void(DynamicData&, const UA_Variant&,
 * cons std::string&)> Provides the appropriate function based on the tye of
 * the scalar value held in the OPC UA Variant.
 */
std::function<void(DynamicData&, const UA_Variant&, const std::string&)>
opc_ua_scalar_variant_to_dds_dynamic_data_fnc(
        const sdk::types::BuiltinTypeKind& type_kind,
        const dds::core::xtypes::TypeKind& dds_type_kind);


/**
 * @brief Set a DynamicData member with the value of an OPC UA Variant. This
 * method supports both scalar and array Variant values.
 *
 * This method is only activated if the DDS_TYPE template parameter is of
 * an arithmetic type. In that case, the value of the variant can be simply
 * assigned to the vector holding the variant values(s).
 *
 * @tparam OPCUA_TYPE OPC UA type selected in the UA_Variant.
 * @tparam DDS_TYPE DDS type associated with the DynamicData member to be set.
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source variant.
 * @param dd_member_name Name of the member to be set within the DynamicData
 * object.
 * @return std::enable_if<std::is_arithmetic<DDS_TYPE>::value, void>::type
 * Activates the method based on whether DDS_TYPE is arithmetic. The return
 * type is void.
 */
template<typename OPCUA_TYPE, typename DDS_TYPE>
typename std::enable_if<std::is_arithmetic<DDS_TYPE>::value, void>::type
opc_ua_variant_value_to_dds_dynamic_data_variant_value(
        DynamicData& dd_value,
        const UA_Variant& opcua_value,
        const std::string& dd_member_name)
{
    // If the variant contains a scalar copy 1 element, otherwise copy
    // opcua_value.arrayLength elements
    size_t array_length =
            UA_Variant_isScalar(&opcua_value) ? 1 : opcua_value.arrayLength;

    std::vector<DDS_TYPE> v;
    v.assign(
            static_cast<DDS_TYPE*>(opcua_value.data),
            static_cast<DDS_TYPE*>(opcua_value.data) + array_length);

    // To workaround CORE-8061 when dealing with arrays within unions we must
    // call loan_value on the member before setting its values.
    dd_value.loan_value(dd_member_name);
    dd_value.set_values(dd_member_name, v);
}


/**
 * @brief Set a DynamicData member with the value of an OPC UA Variant. This
 * method supports both scalar and array Variant values.
 *
 * This method is only activated if the DDS_TYPE template parameter is not of
 * an arithmetic type. In that case, the method iterates every every element
 * of the OPC UA Variant (once for scalar values) and assigns the corresponding
 * element to the corresponding element in the DynamicData sequence holding the
 * Variant value.
 *
 * @tparam OPCUA_TYPE OPC UA type selected in the UA_Variant.
 * @tparam DDS_TYPE DDS type associated with the DynamicData member to be set.
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source variant.
 * @param dd_member_name Name of the member to be set within the DynamicData
 * object.
 * @return std::enable_if<std::is_arithmetic<DDS_TYPE>::value, void>::type
 * Activates the method based on whether DDS_TYPE is not arithmetic. The return
 * type is void.
 */
template<typename OPCUA_TYPE, typename DDS_TYPE>
typename std::enable_if<!std::is_arithmetic<DDS_TYPE>::value, void>::type
opc_ua_variant_value_to_dds_dynamic_data_variant_value(
        DynamicData& dd_value,
        const UA_Variant& opcua_value,
        const std::string& dd_member_name)
{
    // If the variant contains a scalar iterate once, otherwise iterate
    // opcua_value.arrayLength times
    uint32_t array_length = UA_Variant_isScalar(&opcua_value)
            ? 1
            : static_cast<uint32_t>(opcua_value.arrayLength);

    LoanedDynamicData variant_array = dd_value.loan_value(dd_member_name);

    for (uint32_t i = 0; i < array_length; i++) {
        OPCUA_TYPE* opcua_data = static_cast<OPCUA_TYPE*>(opcua_value.data);
        set_dynamic_data_member<OPCUA_TYPE, DDS_TYPE>(
                variant_array.get(),
                &opcua_data[i],
                i + 1);  // Member IDs for sequences are 1-indexed, so we need
                         // to start with i=1
    }
}

/**
 * @brief Set array_dimensions member of an DynamicData object representing an
 * OPC UA Variant with the ArrayDimensions of an actual OPC UA Variant.
 *
 * @param dynamicdata_variant DynamicData member representing an OPC UA Variant.
 * @param opcua_variant OPC UA Variant containing the ArrayDimensions array to
 * be set in the DynamicData object.
 */
void opc_ua_array_dimensions_to_dds_dynamic_data_array_dimensions(
        DynamicData& dynamicdata_variant,
        const opcua::sdk::types::Variant& opcua_variant);

/**
 * @brief Set a member of a DynamicData object with the value of an OPC UA
 * Variant containing an array value.
 *
 * @tparam OPCUA_TYPE OPC UA type selected in the UA_Variant.
 * @tparam DDS_TYPE DDS type associated with the DynamicData member to be set.
 * @param dd_value DynamicData member to be set.
 * @param opcua_value OPC UA source variant.
 * @param dd_member_name DDS member to be set within the DynamicData object.
 */
template<typename OPCUA_TYPE, typename DDS_TYPE>
void opc_ua_variant_scalar_to_dds_dynamic_data_sequence_type(
        DynamicData& dd_value,
        const UA_Variant& opcua_value,
        const std::string& dd_member_name)
{
    opc_ua_variant_value_to_dds_dynamic_data_variant_value<
            OPCUA_TYPE,
            DDS_TYPE>(dd_value, opcua_value, dd_member_name);
}

/**
 * @brief Function wrapper that provides access to the appropriate function to
 * set a DDS DynamicData member with the array value stored in an OPC UA
 * Variant.
 *
 * This method is useful in static functions, such as on_data_available
 * callbacks, that need to set the value of a DynamicData member without
 * much context of the value to be transformed.
 *
 * @param type_kind Type kind of the value held in the OPC UA Variant.
 * @param dds_type_kind Type kind of the elements held in the DDS Dynamic Data
 * member.
 * @return std::function<void(DynamicData&, const UA_Variant&,
 * cons std::string&)> Provides the appropriate function based on the tye of
 * the scalar value held in the OPC UA Variant.
 */
std::function<void(DynamicData&, const UA_Variant&, const std::string&)>
opc_ua_array_variant_to_dds_dynamic_data_fnc(
        const sdk::types::BuiltinTypeKind& opcua_type_kind,
        const dds::core::xtypes::TypeKind& dds_type_kind);

/**
 * @brief Set variant_value member of an DynamicData object representing an
 * OPC UA Variant with the actual OPC UA Variant value.
 *
 * @param dynamicdata_variant DynamicData member representing an OPC UA Variant.
 * @param opcua_variant OPC UA Variant containing the value to be set in the
 * DynamicData object.
 */
void opc_ua_variant_value_to_dds_dynamic_data_variant_value(
        DynamicData& dynamicdata_variant,
        const opcua::sdk::types::Variant& opcua_variant);

/**
 * @brief Set a DynamicData object representing an OPC UA Variant with the
 * actual OPC UA Variant.
 *
 * @param dynamicdata_variant DynamicData member representing an OPC UA Variant.
 * @param opcua_variant OPC UA Variant containing the value to be set in the
 * DynamicData object.
 */
void opc_ua_variant_to_dds_dynamic_data_variant_type(
        DynamicData& dynamicdata_variant,
        const opcua::sdk::types::Variant& opcua_variant);

/**
 * @brief Set a vector of 8-bit unsigned integers with the value of an
 * OPC UA ByteString.
 *
 * @param v Vector to be set.
 * @param opcua_value UA_ByteString containing the values to be set in the
 * vector.
 */
inline void opc_ua_bytestring_to_uint8_vector(
        std::vector<uint8_t>& v,
        const UA_ByteString* opcua_value);

void opc_ua_date_time_to_dds_time_t(
        DDS_Time_t& dd_value,
        const UA_DateTime& opc_ua_data);

void opc_ua_node_id_to_dds_dynamic_data(
        DynamicData& dd_nodeid,
        const UA_NodeId* opcua_nodeid);

void opc_ua_guid_to_dds_dynamic_data(
        DynamicData& dd_guid,
        const UA_Guid* opcua_guid);

void opc_ua_expanded_node_id_to_dds_dynamic_data(
        DynamicData& dd_expanded_nodeid,
        const UA_ExpandedNodeId* opcua_expandednodeid);


}}}  // namespace rti::ddsopcua::conversion

#endif  // RTI_CONVERSIONS_OPCUA2DDSDYNAMICDATACONVERSIONS_HPP_
