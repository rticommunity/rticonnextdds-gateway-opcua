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

#include <thread>

#include <dds/core/ddscore.hpp>
#include <dds/sub/ddssub.hpp>
#include <gtest/gtest.h>

#include <rti/ddsopcua/DdsOpcUaGateway.hpp>
#include <rti/ddsopcua/requester/DdsRequester.hpp>

#include "tutorials/ddsPublisher/DdsDemoPublisher.hpp"
#include "IntegrationTestTypes.hpp"
#include "OpcUaTutorialServer.hpp"

namespace rti { namespace ddsopcua { namespace test {

const int MAX_RETRIES = 5;

class OpcUaDdsTutorialGateway {
public:
    OpcUaDdsTutorialGateway(const std::string& cfg_name) : gateway_(nullptr)
    {
        rti::ddsopcua::Gateway::initialize_globals();
        gateway_ = new rti::ddsopcua::Gateway(
                rti::ddsopcua::GatewayProperty(cfg_name));
    }

    ~OpcUaDdsTutorialGateway()
    {
        gateway_->stop();
        delete gateway_;
        rti::ddsopcua::Gateway::finalize_globals();
    }

    void start()
    {
        gateway_->start();
    }

    void stop()
    {
        gateway_->stop();
    }

private:
    rti::ddsopcua::Gateway* gateway_;
};


template<typename T>
int32_t wait_for_matching_subscription(dds::pub::DataWriter<T>& writer)
{
    dds::core::status::PublicationMatchedStatus pub_status;
    for (int i = 0; i < MAX_RETRIES && pub_status.current_count() < 1;
         i++, rti::util::sleep(dds::core::Duration(1))) {
        pub_status = writer.publication_matched_status();
    }
    return pub_status.current_count();
}

template<typename T>
int32_t wait_for_matching_publication(dds::sub::DataReader<T>& reader)
{
    dds::core::status::SubscriptionMatchedStatus sub_status;
    for (int i = 0; i < MAX_RETRIES && sub_status.current_count() < 1;
         i++, rti::util::sleep(dds::core::Duration(1))) {
        sub_status = reader.subscription_matched_status();
    }
    return sub_status.current_count();
}

bool send_read_request(
        opcua::types::Variant& variant,
        rti::ddsopcua::requester::Requester& requester,
        const std::string& server_id,
        uint16_t namespace_index,
        uint32_t numeric_identifier)
{
    rti::opcua::types::NodeId node_id;

    node_id.namespace_index(namespace_index);
    node_id.identifier_type().numeric_id(numeric_identifier);

    if (requester.read_request(variant, server_id, node_id) != 0) {
        return false;
    }

    return true;
}

bool assert_string_values(
        rti::ddsopcua::requester::Requester& requester,
        const std::string& server_id,
        uint16_t namespace_index,
        uint32_t numeric_identifier,
        const std::string& value,
        size_t size)
{
    opcua::types::Variant variant;

    if (!send_read_request(
                variant,
                requester,
                server_id,
                namespace_index,
                numeric_identifier)) {
        return false;
    }

    if (variant.value().string_value().size() != size) {
        return false;
    }

    for (auto element : variant.value().string_value()) {
        if (element != value) {
            return false;
        }
    }

    return true;
}

bool assert_int32_values(
        rti::ddsopcua::requester::Requester& requester,
        const std::string& server_id,
        uint16_t namespace_index,
        uint32_t numeric_identifier,
        int32_t value,
        size_t size)
{
    opcua::types::Variant variant;
    if (!send_read_request(
                variant,
                requester,
                server_id,
                namespace_index,
                numeric_identifier)) {
        return false;
    }
    if (variant.value().int32_value().size() != size) {
        return false;
    }
    for (auto element : variant.value().int32_value()) {
        if (element != value) {
            return false;
        }
    }

    return true;
}

bool assert_int16_values(
        rti::ddsopcua::requester::Requester& requester,
        const std::string& server_id,
        uint16_t namespace_index,
        uint32_t numeric_identifier,
        int16_t value,
        size_t size)
{
    opcua::types::Variant variant;
    if (!send_read_request(
                variant,
                requester,
                server_id,
                namespace_index,
                numeric_identifier)) {
        return false;
    }
    if (variant.value().int16_value().size() != size) {
        return false;
    }
    for (auto element : variant.value().int16_value()) {
        if (element != value) {
            return false;
        }
    }

    return true;
}

bool assert_bool_values(
        rti::ddsopcua::requester::Requester& requester,
        const std::string& server_id,
        uint16_t namespace_index,
        uint32_t numeric_identifier,
        bool value,
        size_t size)
{
    opcua::types::Variant variant;
    if (!send_read_request(
                variant,
                requester,
                server_id,
                namespace_index,
                numeric_identifier)) {
        return false;
    }

    if (variant.value().bool_value().size() != size) {
        return false;
    }

    for (auto element : variant.value().bool_value()) {
        if (element != value) {
            return false;
        }
    }

    return true;
}

TEST(IntegrationTests, Tutorial1)
{
    bool running = true;

    // Instantiate OPC UA Server
    OpcUaTutorialServer server;
    server.start();

    // Instantiate OPC UA/DDS Gateway
    OpcUaDdsTutorialGateway gateway("default");
    gateway.start();

    // Instantiate DDS DataReader
    dds::domain::qos::DomainParticipantQos participant_qos;
    participant_qos.policy<rti::core::policy::Database>()
            .shutdown_cleanup_period(dds::core::Duration(0, 100000000));

    dds::domain::DomainParticipant participant(0, participant_qos);
    dds::topic::Topic<ScalarTypes> topic(participant, "ScalarTypesTopic");
    uint16_t samples_read = 0;

    dds::sub::DataReader<ScalarTypes> reader(
            dds::sub::Subscriber(participant),
            topic);

    dds::sub::cond::ReadCondition read_condition(
            reader,
            dds::sub::status::DataState::any_data(),
            [&reader, &samples_read]() {
                dds::sub::LoanedSamples<ScalarTypes> samples = reader.take();
                for (const auto& sample : samples) {
                    if (sample.info().valid()) {
                        samples_read++;
                        std::cout << sample.data() << std::endl;
                    }
                }
            });

    dds::core::cond::WaitSet waitset;
    waitset += read_condition;

    // Write and read 3 samples
    uint16_t samples_to_read = 3;
    uint16_t remaining_samples = samples_to_read;
    ASSERT_GE(wait_for_matching_publication<ScalarTypes>(reader), 1);

    while (remaining_samples > 0) {
        server.update_nodes();

        waitset.dispatch(dds::core::Duration(10));
        remaining_samples--;
    }
    // Check we've read all the samples we've written
    ASSERT_EQ(samples_read, samples_to_read);

    gateway.stop();
    server.stop();
}

TEST(IntegrationTests, Tutorial2)
{
    OpcUaTutorialServer server;
    server.start();

    OpcUaDdsTutorialGateway gateway("default");
    gateway.start();

    rti::ddsopcua::requester::Requester requester(0);
    bool discovered_gateway = requester.wait_for_matching_gateway(10);
    ASSERT_EQ(discovered_gateway, true);

    // Read existing nodes (array and scalar nodes)
    std::string server_id = "MyServerConnection";
    rti::opcua::types::NodeId node_id;
    node_id.namespace_index(1);
    rti::opcua::types::Variant value;
    std::vector<uint32_t> nodes_to_check { 52001, 52012, 52101, 52112 };
    for (auto node : nodes_to_check) {
        node_id.identifier_type().numeric_id(node);
        uint32_t status_code =
                requester.read_request(value, server_id, node_id, 10);
        ASSERT_EQ(status_code, 0);
    }

    // Read non-existing node
    node_id.namespace_index(100);
    uint32_t status_code =
            requester.read_request(value, server_id, node_id, 10);
    ASSERT_NE(status_code, 0);

    // write a boolean
    node_id.namespace_index(1);
    node_id.identifier_type().numeric_id(51001);
    value.value().bool_value({ true });
    status_code = requester.write_request(server_id, node_id, value, 10);
    ASSERT_EQ(status_code, 0);

    ASSERT_TRUE(assert_bool_values(requester, server_id, 1, 51001, true, 1));

    // write a boolean array
    node_id.identifier_type().numeric_id(51101);
    value.array_dimensions({ 0 });
    value.value().bool_value(
            { true, true, true, true, true, true, true, true, true, true });
    status_code = requester.write_request(server_id, node_id, value);
    ASSERT_EQ(status_code, 0);

    ASSERT_TRUE(assert_bool_values(requester, server_id, 1, 51101, true, 10));

    // write a string
    node_id.identifier_type().numeric_id(51012);
    value.array_dimensions({});
    value.value().string_value({ "Odd" });
    status_code = requester.write_request(server_id, node_id, value);
    ASSERT_EQ(status_code, 0);

    ASSERT_TRUE(assert_string_values(requester, server_id, 1, 51012, "Odd", 1));

    // write a string array
    node_id.identifier_type().numeric_id(51112);
    value.array_dimensions({ 0 });
    value.value().string_value(
            { "Odd", "Odd", "Odd", "Odd", "Odd", "Odd", "Odd", "Odd", "Odd" });
    status_code = requester.write_request(server_id, node_id, value);
    ASSERT_EQ(status_code, 0);

    ASSERT_TRUE(assert_string_values(requester, server_id, 1, 51112, "Odd", 9));

    // write non-existing nodes
    node_id.namespace_index(100);
    node_id.identifier_type().numeric_id(51112);
    value.array_dimensions({ 0 });
    value.value().string_value(
            { "Odd", "Odd", "Odd", "Odd", "Odd", "Odd", "Odd", "Odd", "Odd" });
    status_code = requester.write_request(server_id, node_id, value);
    ASSERT_NE(status_code, 0);

    gateway.stop();
    rti::ddsopcua::Gateway::finalize_globals();
}

TEST(IntegrationTests, Tutorial3)
{
    OpcUaTutorialServer server;
    server.start();

    OpcUaDdsTutorialGateway gateway("publicationExample");
    gateway.start();

    dds::domain::qos::DomainParticipantQos participant_qos;
    participant_qos.policy<rti::core::policy::Database>()
            .shutdown_cleanup_period(dds::core::Duration(0, 100000000));

    rti::ddsopcua::requester::Requester requester(0, participant_qos);
    bool discovered_gateway = requester.wait_for_matching_gateway(10);
    ASSERT_EQ(discovered_gateway, true);

    dds::domain::DomainParticipant participant(0, participant_qos);

    dds::core::xtypes::StructType type = tutorials::create_type();
    dds::topic::Topic<dds::core::xtypes::DynamicData> topic(
            participant,
            "MyTopic",
            type);
    dds::pub::DataWriter<dds::core::xtypes::DynamicData> writer(
            dds::pub::Publisher(participant),
            topic);
    dds::core::xtypes::DynamicData sample(type);

    ASSERT_GE(
            wait_for_matching_subscription<dds::core::xtypes::DynamicData>(
                    writer),
            1);

    // Write in DDS and check integrity in OPC UA Server
    tutorials::update_sample(sample, 1);
    writer.write(sample);
    writer.wait_for_acknowledgments(dds::core::Duration(10));

    std::string server_id = "MyServerConnection";
    opcua::types::Variant value;

    bool read_values = false;
    for (int i = 0; i < MAX_RETRIES && !read_values;
         i++, rti::util::sleep(dds::core::Duration(1))) {
        read_values =
                assert_bool_values(requester, server_id, 1, 61103, true, 1);
    }
    ASSERT_TRUE(read_values);
    ASSERT_TRUE(assert_bool_values(
            requester,
            server_id,
            1,
            61115,
            true,
            tutorials::MAX_LENGTH));

    ASSERT_TRUE(assert_int16_values(requester, server_id, 1, 61105, 1, 1));
    ASSERT_TRUE(assert_int16_values(
            requester,
            server_id,
            1,
            61117,
            1,
            tutorials::MAX_LENGTH));

    // Write again in DDS and check integrity in OPC UA Server

    tutorials::update_sample(sample, 2);
    writer.write(sample);
    writer.wait_for_acknowledgments(dds::core::Duration(10));

    read_values = false;
    for (int i = 0; i < MAX_RETRIES && !read_values;
         i++, rti::util::sleep(dds::core::Duration(1))) {
        read_values =
                assert_bool_values(requester, server_id, 1, 61103, false, 1);
    }
    ASSERT_TRUE(read_values);
    ASSERT_TRUE(assert_bool_values(
            requester,
            server_id,
            1,
            61115,
            false,
            tutorials::MAX_LENGTH));

    ASSERT_TRUE(assert_int16_values(requester, server_id, 1, 61105, -2, 1));
    ASSERT_TRUE(assert_int16_values(
            requester,
            server_id,
            1,
            61117,
            -2,
            tutorials::MAX_LENGTH));
}

TEST(IntegrationTests, Tutorial4)
{
    OpcUaTutorialServer server;
    server.start();

    OpcUaDdsTutorialGateway gateway("publicationExample");
    gateway.start();

    dds::domain::qos::DomainParticipantQos participant_qos;
    participant_qos.policy<rti::core::policy::Database>()
            .shutdown_cleanup_period(dds::core::Duration(0, 100000000));

    rti::ddsopcua::requester::Requester requester(0, participant_qos);
    bool discovered_gateway = requester.wait_for_matching_gateway(10);
    ASSERT_EQ(discovered_gateway, true);

    dds::domain::DomainParticipant participant(0, participant_qos);
    dds::topic::Topic<ShapeType> topic(participant, "Square");

    dds::pub::DataWriter<ShapeType> writer(
            dds::pub::Publisher(participant),
            topic);

    ASSERT_GE(wait_for_matching_subscription<ShapeType>(writer), 1);

    ShapeType sample;

    // Publish BLUE Square

    sample.color("BLUE");
    sample.x(42);
    sample.y(42);
    sample.shapesize(30);

    writer.write(sample);
    writer.wait_for_acknowledgments(dds::core::Duration(10));

    std::string server_id = "MyServerConnection";

    bool read_values = false;
    for (int i = 0; i < MAX_RETRIES && !read_values;
         i++, rti::util::sleep(dds::core::Duration(1))) {
        read_values =
                assert_string_values(requester, server_id, 1, 60103, "BLUE", 1);
    }
    ASSERT_TRUE(read_values);
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60104, 42, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60104, 42, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60105, 42, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60106, 30, 1));

    ASSERT_TRUE(
            assert_string_values(requester, server_id, 1, 60303, "BLUE", 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60304, 42, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60304, 42, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60305, 42, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60306, 30, 1));

    sample.color("RED");
    sample.x(43);
    sample.y(43);
    sample.shapesize(40);

    writer.write(sample);
    writer.wait_for_acknowledgments(dds::core::Duration(10));

    ASSERT_FALSE(
            assert_string_values(requester, server_id, 1, 60103, "RED", 1));
    ASSERT_FALSE(assert_int32_values(requester, server_id, 1, 60104, 43, 1));
    ASSERT_FALSE(assert_int32_values(requester, server_id, 1, 60105, 43, 1));
    ASSERT_FALSE(assert_int32_values(requester, server_id, 1, 60106, 40, 1));

    ASSERT_TRUE(assert_string_values(requester, server_id, 1, 60203, "RED", 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60204, 43, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60205, 43, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60206, 40, 1));

    ASSERT_TRUE(assert_string_values(requester, server_id, 1, 60303, "RED", 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60304, 43, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60305, 43, 1));
    ASSERT_TRUE(assert_int32_values(requester, server_id, 1, 60306, 40, 1));
}

TEST(IntegrationTests, MultipleSubscriptions)
{
    bool running = true;

    // Instantiate OPC UA Server
    OpcUaTutorialServer server;
    server.start();

    // Instantiate OPC UA/DDS Gateway
    OpcUaDdsTutorialGateway gateway("multipleSubscriptions");
    gateway.start();

    // Instantiate DDS DataReader
    dds::domain::qos::DomainParticipantQos participant_qos;
    participant_qos.policy<rti::core::policy::Database>()
            .shutdown_cleanup_period(dds::core::Duration(0, 100000000));

    dds::domain::DomainParticipant participant(0, participant_qos);
    dds::topic::Topic<ScalarTypes> topic(participant, "ScalarTypesTopic");
    uint16_t samples_read = 0;

    dds::sub::DataReader<ScalarTypes> reader(
            dds::sub::Subscriber(participant),
            topic);

    dds::sub::cond::ReadCondition read_condition(
            reader,
            dds::sub::status::DataState::any_data(),
            [&reader, &samples_read]() {
                dds::sub::LoanedSamples<ScalarTypes> samples = reader.take();
                for (const auto& sample : samples) {
                    if (sample.info().valid()) {
                        samples_read++;
                        std::cout << sample.data() << std::endl;
                    }
                }
            });

    dds::core::cond::WaitSet waitset;
    waitset += read_condition;

    // Write and read 3 samples
    uint16_t samples_to_read = 3;
    uint16_t remaining_samples = samples_to_read;
    ASSERT_GE(wait_for_matching_publication<ScalarTypes>(reader), 1);

    while (remaining_samples > 0) {
        server.update_nodes();

        waitset.dispatch(dds::core::Duration(10));
        remaining_samples--;
    }
    // Check we've read all the samples we've written
    ASSERT_EQ(samples_read, samples_to_read);

    gateway.stop();
    server.stop();
}

}}}  // namespace rti::ddsopcua::test
