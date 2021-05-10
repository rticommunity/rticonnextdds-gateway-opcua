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

#include <gtest/gtest.h>

#include "opcUaSdk/OpcUaSdkDataTypes.hpp"

TEST(OpcUaDataTypesTester, AttributeIdTransformations)
{
    EXPECT_THROW(
            { rti::opcua::sdk::types::AttributeId id(0); },
            rti::ddsopcua::GatewayException);
    EXPECT_THROW(
            { rti::opcua::sdk::types::AttributeId id(24); },
            rti::ddsopcua::GatewayException);
    EXPECT_THROW(
            { rti::opcua::sdk::types::AttributeId id("Fake"); },
            rti::ddsopcua::GatewayException);
    {
        rti::opcua::sdk::types::AttributeId id(1);
        ASSERT_EQ(id.value(), 1);
        ASSERT_EQ(id.to_string(), "NODEID");
    }
    {
        rti::opcua::sdk::types::AttributeId id("NODEID");
        ASSERT_EQ(id.value(), 1);
        ASSERT_EQ(id.to_string(), "NODEID");
    }
    {
        rti::opcua::sdk::types::AttributeId id(2);
        ASSERT_EQ(id.value(), 2);
        ASSERT_EQ(id.to_string(), "NODECLASS");
    }
    {
        rti::opcua::sdk::types::AttributeId id("NODECLASS");
        ASSERT_EQ(id.value(), 2);
        ASSERT_EQ(id.to_string(), "NODECLASS");
    }
    {
        rti::opcua::sdk::types::AttributeId id(3);
        ASSERT_EQ(id.value(), 3);
        ASSERT_EQ(id.to_string(), "BROWSENAME");
    }
    {
        rti::opcua::sdk::types::AttributeId id("BROWSENAME");
        ASSERT_EQ(id.value(), 3);
        ASSERT_EQ(id.to_string(), "BROWSENAME");
    }
    {
        rti::opcua::sdk::types::AttributeId id(4);
        ASSERT_EQ(id.value(), 4);
        ASSERT_EQ(id.to_string(), "DISPLAYNAME");
    }
    {
        rti::opcua::sdk::types::AttributeId id("DISPLAYNAME");
        ASSERT_EQ(id.value(), 4);
        ASSERT_EQ(id.to_string(), "DISPLAYNAME");
    }
    {
        rti::opcua::sdk::types::AttributeId id(5);
        ASSERT_EQ(id.value(), 5);
        ASSERT_EQ(id.to_string(), "DESCRIPTION");
    }
    {
        rti::opcua::sdk::types::AttributeId id("DESCRIPTION");
        ASSERT_EQ(id.value(), 5);
        ASSERT_EQ(id.to_string(), "DESCRIPTION");
    }
    {
        rti::opcua::sdk::types::AttributeId id(6);
        ASSERT_EQ(id.value(), 6);
        ASSERT_EQ(id.to_string(), "WRITEMASK");
    }
    {
        rti::opcua::sdk::types::AttributeId id("WRITEMASK");
        ASSERT_EQ(id.value(), 6);
        ASSERT_EQ(id.to_string(), "WRITEMASK");
    }
    {
        rti::opcua::sdk::types::AttributeId id(7);
        ASSERT_EQ(id.value(), 7);
        ASSERT_EQ(id.to_string(), "USERWRITEMASK");
    }
    {
        rti::opcua::sdk::types::AttributeId id("USERWRITEMASK");
        ASSERT_EQ(id.value(), 7);
        ASSERT_EQ(id.to_string(), "USERWRITEMASK");
    }
    {
        rti::opcua::sdk::types::AttributeId id(8);
        ASSERT_EQ(id.value(), 8);
        ASSERT_EQ(id.to_string(), "ISABSTRACT");
    }
    {
        rti::opcua::sdk::types::AttributeId id("ISABSTRACT");
        ASSERT_EQ(id.value(), 8);
        ASSERT_EQ(id.to_string(), "ISABSTRACT");
    }
    {
        rti::opcua::sdk::types::AttributeId id(9);
        ASSERT_EQ(id.value(), 9);
        ASSERT_EQ(id.to_string(), "SYMMETRIC");
    }
    {
        rti::opcua::sdk::types::AttributeId id("SYMMETRIC");
        ASSERT_EQ(id.value(), 9);
        ASSERT_EQ(id.to_string(), "SYMMETRIC");
    }
    {
        rti::opcua::sdk::types::AttributeId id(10);
        ASSERT_EQ(id.value(), 10);
        ASSERT_EQ(id.to_string(), "INVERSENAME");
    }
    {
        rti::opcua::sdk::types::AttributeId id("INVERSENAME");
        ASSERT_EQ(id.value(), 10);
        ASSERT_EQ(id.to_string(), "INVERSENAME");
    }
    {
        rti::opcua::sdk::types::AttributeId id(11);
        ASSERT_EQ(id.value(), 11);
        ASSERT_EQ(id.to_string(), "CONTAINSNOLOOPS");
    }
    {
        rti::opcua::sdk::types::AttributeId id("CONTAINSNOLOOPS");
        ASSERT_EQ(id.value(), 11);
        ASSERT_EQ(id.to_string(), "CONTAINSNOLOOPS");
    }
    {
        rti::opcua::sdk::types::AttributeId id(12);
        ASSERT_EQ(id.value(), 12);
        ASSERT_EQ(id.to_string(), "EVENTNOTIFIER");
    }
    {
        rti::opcua::sdk::types::AttributeId id("EVENTNOTIFIER");
        ASSERT_EQ(id.value(), 12);
        ASSERT_EQ(id.to_string(), "EVENTNOTIFIER");
    }
    {
        rti::opcua::sdk::types::AttributeId id(13);
        ASSERT_EQ(id.value(), 13);
        ASSERT_EQ(id.to_string(), "VALUE");
    }
    {
        rti::opcua::sdk::types::AttributeId id("VALUE");
        ASSERT_EQ(id.value(), 13);
        ASSERT_EQ(id.to_string(), "VALUE");
    }
    {
        rti::opcua::sdk::types::AttributeId id(14);
        ASSERT_EQ(id.value(), 14);
        ASSERT_EQ(id.to_string(), "DATATYPE");
    }
    {
        rti::opcua::sdk::types::AttributeId id("DATATYPE");
        ASSERT_EQ(id.value(), 14);
        ASSERT_EQ(id.to_string(), "DATATYPE");
    }
    {
        rti::opcua::sdk::types::AttributeId id(15);
        ASSERT_EQ(id.value(), 15);
        ASSERT_EQ(id.to_string(), "VALUERANK");
    }
    {
        rti::opcua::sdk::types::AttributeId id("VALUERANK");
        ASSERT_EQ(id.value(), 15);
        ASSERT_EQ(id.to_string(), "VALUERANK");
    }
    {
        rti::opcua::sdk::types::AttributeId id(16);
        ASSERT_EQ(id.value(), 16);
        ASSERT_EQ(id.to_string(), "ARRAYDIMENSIONS");
    }
    {
        rti::opcua::sdk::types::AttributeId id("ARRAYDIMENSIONS");
        ASSERT_EQ(id.value(), 16);
        ASSERT_EQ(id.to_string(), "ARRAYDIMENSIONS");
    }
    {
        rti::opcua::sdk::types::AttributeId id(17);
        ASSERT_EQ(id.value(), 17);
        ASSERT_EQ(id.to_string(), "ACCESSLEVEL");
    }
    {
        rti::opcua::sdk::types::AttributeId id("ACCESSLEVEL");
        ASSERT_EQ(id.value(), 17);
        ASSERT_EQ(id.to_string(), "ACCESSLEVEL");
    }
    {
        rti::opcua::sdk::types::AttributeId id(18);
        ASSERT_EQ(id.value(), 18);
        ASSERT_EQ(id.to_string(), "USERACCESSLEVEL");
    }
    {
        rti::opcua::sdk::types::AttributeId id("USERACCESSLEVEL");
        ASSERT_EQ(id.value(), 18);
        ASSERT_EQ(id.to_string(), "USERACCESSLEVEL");
    }
    {
        rti::opcua::sdk::types::AttributeId id(19);
        ASSERT_EQ(id.value(), 19);
        ASSERT_EQ(id.to_string(), "MINIMUMSAMPLINGINTERVAL");
    }
    {
        rti::opcua::sdk::types::AttributeId id("MINIMUMSAMPLINGINTERVAL");
        ASSERT_EQ(id.value(), 19);
        ASSERT_EQ(id.to_string(), "MINIMUMSAMPLINGINTERVAL");
    }
    {
        rti::opcua::sdk::types::AttributeId id(20);
        ASSERT_EQ(id.value(), 20);
        ASSERT_EQ(id.to_string(), "HISTORIZING");
    }
    {
        rti::opcua::sdk::types::AttributeId id("HISTORIZING");
        ASSERT_EQ(id.value(), 20);
        ASSERT_EQ(id.to_string(), "HISTORIZING");
    }
    {
        rti::opcua::sdk::types::AttributeId id(21);
        ASSERT_EQ(id.value(), 21);
        ASSERT_EQ(id.to_string(), "EXECUTABLE");
    }
    {
        rti::opcua::sdk::types::AttributeId id("EXECUTABLE");
        ASSERT_EQ(id.value(), 21);
        ASSERT_EQ(id.to_string(), "EXECUTABLE");
    }
    {
        rti::opcua::sdk::types::AttributeId id(22);
        ASSERT_EQ(id.value(), 22);
        ASSERT_EQ(id.to_string(), "USEREXECUTABLE");
    }
    {
        rti::opcua::sdk::types::AttributeId id("USEREXECUTABLE");
        ASSERT_EQ(id.value(), 22);
        ASSERT_EQ(id.to_string(), "USEREXECUTABLE");
    }
    {
        rti::opcua::sdk::types::AttributeId id(23);
        ASSERT_EQ(id.value(), 23);
        ASSERT_EQ(id.to_string(), "DATATYPEDEFINITION");
    }
    {
        rti::opcua::sdk::types::AttributeId id("DATATYPEDEFINITION");
        ASSERT_EQ(id.value(), 23);
        ASSERT_EQ(id.to_string(), "DATATYPEDEFINITION");
    }
}

TEST(OpcUaDataTypesTester, NodeIdTransformations)
{
    {
        rti::opcua::sdk::types::NodeId node_id;
		ASSERT_NE(node_id.get_const_ref(), nullptr);
    }
    {
        rti::opcua::sdk::types::NodeId source(1, 1001);
        rti::opcua::sdk::types::NodeId dst(source);
        ASSERT_EQ(dst.get_const_ref()->namespaceIndex, 1);
        ASSERT_EQ(dst.get_const_ref()->identifier.numeric, 1001);
    }
    {
        rti::opcua::sdk::types::NodeId source(1, 1001);
        rti::opcua::sdk::types::NodeId dst = source;
        ASSERT_EQ(dst.get_const_ref()->namespaceIndex, 1);
        ASSERT_EQ(dst.get_const_ref()->identifier.numeric, 1001);
    }
    {
        rti::opcua::sdk::types::NodeId node_id(1, 1001);
        ASSERT_EQ(node_id.get_const_ref()->namespaceIndex, 1);
        ASSERT_EQ(node_id.get_const_ref()->identifier.numeric, 1001);
        ASSERT_EQ(
                node_id.get_const_ref()->identifierType,
                UA_NODEIDTYPE_NUMERIC);
    }
    {
        std::string string_id("StringIdentifier");
        rti::opcua::sdk::types::NodeId node_id(1, string_id);
        ASSERT_EQ(node_id.get_const_ref()->namespaceIndex, 1);
        UA_String ua_string_id =
                UA_STRING(const_cast<char*>(string_id.c_str()));
        ASSERT_TRUE(UA_String_equal(
                &node_id.get_const_ref()->identifier.string,
                &ua_string_id));
        ASSERT_EQ(
                node_id.get_const_ref()->identifierType,
                UA_NODEIDTYPE_STRING);
    }
    {
        rti::opcua::sdk::types::NodeId node_id(1, 1001);
        std::string node_id_str = node_identifier_to_string(node_id);
        ASSERT_EQ(node_id_str, "1001");
    }
    {
        std::string string_id("StringIdentifier");
        rti::opcua::sdk::types::NodeId node_id(1, string_id);
        std::string node_id_str = node_identifier_to_string(node_id);
        ASSERT_EQ(node_id_str, string_id);
    }
    {
        const char* bytestring = "Bytestring";
        UA_NodeId ua_node_id =
                UA_NODEID_BYTESTRING(1, const_cast<char*>(bytestring));
        rti::opcua::sdk::types::NodeId node_id(ua_node_id);
        std::string node_id_str = node_identifier_to_string(node_id);
        ASSERT_EQ(node_id_str, "");
    }
}

TEST(OpcUaDataTypesTester, VariantTransformations)
{
    {
        rti::opcua::sdk::types::Variant variant;
        ASSERT_EQ(
                variant.get_type_kind(),
                rti::opcua::sdk::types::BuiltinTypeKind::VARIANT_TYPE);
    }
    {
        UA_ExtensionObject extension_object;
        UA_ExtensionObject_init(&extension_object);

        UA_Variant ua_variant;
        UA_Variant_setScalar(
                &ua_variant,
                &extension_object,
                &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
        rti::opcua::sdk::types::Variant variant(ua_variant);
        ASSERT_EQ(
                variant.get_type_kind(),
                rti::opcua::sdk::types::BuiltinTypeKind::EXTENSIONOBJECT_TYPE);
    }
    {
        UA_DataValue datavalue;
        UA_DataValue_init(&datavalue);

        UA_Variant ua_variant;
        UA_Variant_setScalar(
                &ua_variant,
                &datavalue,
                &UA_TYPES[UA_TYPES_DATAVALUE]);
        rti::opcua::sdk::types::Variant variant(ua_variant);
        ASSERT_EQ(
                variant.get_type_kind(),
                rti::opcua::sdk::types::BuiltinTypeKind::DATAVALUE_TYPE);
    }
    {
        UA_Variant variant_value;
        UA_Variant_init(&variant_value);

        UA_Variant ua_variant;
        UA_Variant_setScalar(
                &ua_variant,
                &variant_value,
                &UA_TYPES[UA_TYPES_VARIANT]);
        rti::opcua::sdk::types::Variant variant(ua_variant);
        ASSERT_EQ(
                variant.get_type_kind(),
                rti::opcua::sdk::types::BuiltinTypeKind::VARIANT_TYPE);
    }
    {
        UA_DiagnosticInfo diagnosticinfo_value;
        UA_DiagnosticInfo_init(&diagnosticinfo_value);

        UA_Variant ua_variant;
        UA_Variant_setScalar(
                &ua_variant,
                &diagnosticinfo_value,
                &UA_TYPES[UA_TYPES_DIAGNOSTICINFO]);
        rti::opcua::sdk::types::Variant variant(ua_variant);
        ASSERT_EQ(
                variant.get_type_kind(),
                rti::opcua::sdk::types::BuiltinTypeKind::DIAGNOSTICINFO_TYPE);
    }
}
