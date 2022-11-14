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

#ifndef RTI_OPCUASDK_CLIENT_HPP_
#define RTI_OPCUASDK_CLIENT_HPP_

#include <open62541/client.h>

#include <dds/core/Duration.hpp>
#include <rti/core/Semaphore.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include "OpcUaSdkClientMonitoredItemProperty.hpp"
#include "OpcUaSdkClientProperty.hpp"
#include "OpcUaSdkClientSubscriptionProperty.hpp"
#include "OpcUaSdkDataTypes.hpp"

namespace rti { namespace opcua { namespace sdk { namespace client {

/**
 * @brief Data notification function to be registered with every MonitoredItem.
 * @param client Pointer to native Client object
 * @param subscription_id Subscription ID associated with the function
 * @param subscription_context Provides a pointer to a subscription context
 * that is registered with the subscription as part of the SubscriptionRequest.
 * @param monitored_item_id ID that was assigned to the monitored item upon
 * its creation.
 * @param value Updated value for a data change.
 */
typedef void (*DataChangeNotificationFnc)(
        UA_Client* client,
        UA_UInt32 subscription_id,
        void* subscription_context,
        UA_UInt32 monitored_item_id,
        void* monitored_item_context,
        UA_DataValue* value);

class Client {
public:
    Client();

    Client(const ClientProperty& configuration);

    ~Client();

    void reset(const ClientProperty& configuration);

    void connect(const std::string& server_uri);

    void disconnect();

    uint32_t create_subscription(
            const SubscriptionProperty& subscription_properties);

    void delete_subscription(const uint32_t& subscription_id);

    uint32_t add_monitored_item(
            const uint32_t& subscription_id,
            const MonitoredNodeAttribute& monitored_item,
            DataChangeNotificationFnc monitoreditem_handling_function,
            void* monitored_item_context);

    bool delete_monitored_item(
            const uint32_t subscription_id,
            const uint32_t monitored_item_id);

    sdk::types::BuiltinTypeKind read_value_attribute_type_kind(
            const sdk::types::NodeId& node_id);

    void read_value_attribute(
            sdk::types::Variant& value_attribute,
            const sdk::types::NodeId& node_id);

    void write_value_attribute(
            const sdk::types::NodeId& node_id,
            const sdk::types::Variant& value);

    void run_iterate(uint16_t timeout);

private:
    UA_Client* client_;
    rti::core::Semaphore client_mutex_;
};

}}}}  // namespace rti::opcua::sdk::client

#endif  // RTI_OPCUASDK_CLIENT_HPP_
