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

#include <open62541/types.h>

#include <dds/core/Time.hpp>
#include <dds/core/types.hpp>
#include <dds/core/xtypes/DynamicData.hpp>

#include "gtest/gtest.h"

#include "conversion/OpcUaBuiltinDataTypes.hpp"
#include "conversion/OpcUa2DdsDynamicDataConversions.hpp"

using namespace rti::ddsopcua::conversion;

TEST(ConversionTests, OpcUaDate2DdsTime)
{
    DDS_Time_t dds_time;
    UA_DateTime opcua_time = 132501796724936540;
    opc_ua_date_time_to_dds_time_t(dds_time, opcua_time);

    ASSERT_EQ(dds_time.sec, 1605706072);
    ASSERT_EQ(dds_time.nanosec, 493654000);
}

TEST(ConversionTests, OpcUaNodeId2DdsDynamicData)
{
    using namespace dds::core::xtypes;
    using namespace rti::opcua::types;

    DynamicData dds_node_id(rti::topic::dynamic_type<NodeId>::get());

    // Numeric NodeId
    UA_NodeId numeric_node = UA_NODEID_NUMERIC(1, 100);
    opc_ua_node_id_to_dds_dynamic_data(dds_node_id, &numeric_node);
    ASSERT_EQ(
            dds_node_id.value<uint16_t>("namespace_index"),
            numeric_node.namespaceIndex);
    ASSERT_EQ(
            dds_node_id.value<uint32_t>("identifier_type.numeric_id"),
            numeric_node.identifier.numeric);

    // String NodeId
    UA_NodeId string_node = UA_NODEID_STRING(2, const_cast<char*>("MyString"));
    opc_ua_node_id_to_dds_dynamic_data(dds_node_id, &string_node);
    ASSERT_EQ(
            dds_node_id.value<uint16_t>("namespace_index"),
            string_node.namespaceIndex);
    ASSERT_EQ(
            dds_node_id.value<std::string>("identifier_type.string_id")
                    .compare(
                            std::string(
                                    reinterpret_cast<const char*>(
                                            string_node.identifier.string.data),
                                    string_node.identifier.string.length)
                                    .c_str()),
            0);

    // ByteString NodeId
    UA_NodeId bytestring_node =
            UA_NODEID_BYTESTRING(3, const_cast<char*>("Odd"));
    opc_ua_node_id_to_dds_dynamic_data(dds_node_id, &bytestring_node);
    ASSERT_EQ(
            dds_node_id.value<uint16_t>("namespace_index"),
            bytestring_node.namespaceIndex);

    std::vector<uint8_t> expected_bytestring = { 79, 100, 100 };
    std::vector<uint8_t> bytestring =
            dds_node_id.get_values<uint8_t>("identifier_type.opaque_id");
    ASSERT_TRUE(std::equal(
            bytestring.begin(),
            bytestring.end(),
            expected_bytestring.begin()));

    // Guid NodeId
    UA_Guid guid_value = { 1, 2, 3, { 4, 5, 6, 7, 8, 9, 10, 11 } };
    UA_NodeId guid_node = UA_NODEID_GUID(3, guid_value);
    opc_ua_node_id_to_dds_dynamic_data(dds_node_id, &guid_node);

    ASSERT_EQ(
            dds_node_id.value<uint16_t>("namespace_index"),
            guid_node.namespaceIndex);
    ASSERT_EQ(
            dds_node_id.value<uint32_t>("identifier_type.guid_id.data1"),
            guid_node.identifier.guid.data1);
    ASSERT_EQ(
            dds_node_id.value<uint16_t>("identifier_type.guid_id.data2"),
            guid_node.identifier.guid.data2);
    ASSERT_EQ(
            dds_node_id.value<uint16_t>("identifier_type.guid_id.data3"),
            guid_node.identifier.guid.data3);
    std::vector<uint8_t> data4_values =
            dds_node_id.get_values<uint8_t>("identifier_type.guid_id.data4");
    ASSERT_EQ(data4_values.size(), 8);
    ASSERT_TRUE(std::equal(
            data4_values.begin(),
            data4_values.end(),
            guid_node.identifier.guid.data4,
            [](uint8_t lhs, uint8_t rhs) { return lhs == rhs; }));
}
