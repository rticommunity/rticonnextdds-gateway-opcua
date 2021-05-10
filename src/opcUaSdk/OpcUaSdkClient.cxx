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

#include <cstddef>

#include <dds/core/Exception.hpp>
#include <rti/util/util.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include <open62541/client.h>
#include <open62541/client_highlevel.h>

#include "OpcUaSdkClient.hpp"
#include "log/LogMsg.hpp"

namespace rti { namespace opcua { namespace sdk { namespace client {

static void client_state_fnc(
        UA_Client* client,
        UA_SecureChannelState channel_state,
        UA_SessionState session_state,
        UA_StatusCode connection_status)
{
    switch (channel_state) {
    case UA_SECURECHANNELSTATE_CLOSED:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "The client is disconnected");
        break;
    case UA_SECURECHANNELSTATE_HEL_SENT:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Waiting for ack");
        break;
    case UA_SECURECHANNELSTATE_OPN_SENT:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Waiting for OPN Response");
        break;
    case UA_SECURECHANNELSTATE_OPEN:
        GATEWAYLog_warn(
                &DDSOPCUA_LOG_ANY_s,
                "A SecureChannel to the server is open");
        break;
    default:
        GATEWAYLog_exception(
                &DDSOPCUA_LOG_ANY_s,
                "Client has reached an unknown state");
    }
    switch (session_state) {
    case UA_SESSIONSTATE_CLOSED:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Session closed");
        break;
    case UA_SESSIONSTATE_CREATE_REQUESTED:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Requested session creation");
        break;
    case UA_SESSIONSTATE_CREATED:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Session created");
        break;
    case UA_SESSIONSTATE_ACTIVATE_REQUESTED:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Session closing");
        break;
    case UA_SESSIONSTATE_ACTIVATED:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Session activated");
        break;
    case UA_SESSIONSTATE_CLOSING:
        GATEWAYLog_warn(&DDSOPCUA_LOG_ANY_s, "Session closing");
        break;
    default:
        GATEWAYLog_exception(
                &DDSOPCUA_LOG_ANY_s,
                "Session has reached an unknown state");
    }
}


Client::Client()
        : client_(UA_Client_new()),
          client_mutex_(RTI_OSAPI_SEMAPHORE_KIND_MUTEX)
{
}

Client::Client(const ClientProperty& property)
        : client_(UA_Client_new()),
          client_mutex_(RTI_OSAPI_SEMAPHORE_KIND_MUTEX)
{
    UA_ClientConfig* client_configuration = UA_Client_getConfig(client_);
    UA_ClientConfig_setDefault(client_configuration);

    property.to_native_configuration(*client_configuration);
    client_configuration->stateCallback = client_state_fnc;
    client_configuration->connectivityCheckInterval = 2000;
}

Client::~Client()
{
    disconnect();

    GATEWAYLog_local(&DDSOPCUA_LOG_ANY_s, "Deleting OPC UA Client");
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);
        UA_Client_delete(client_);
    }

    GATEWAYLog_local(&DDSOPCUA_LOG_ANY_s, "OPC UA Client deleted");
}

void Client::reset(const ClientProperty& configuration)
{
    disconnect();

    GATEWAYLog_local(&DDSOPCUA_LOG_ANY_s, "Reseting OPC UA Client");
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);
        // UA_Client_reset(client_);
        UA_ClientConfig* client_configuration = UA_Client_getConfig(client_);
        UA_ClientConfig_setDefault(client_configuration);

        configuration.to_native_configuration(*client_configuration);
        client_configuration->stateCallback = client_state_fnc;
        client_configuration->connectivityCheckInterval = 2000;
    }

    GATEWAYLog_local(&DDSOPCUA_LOG_ANY_s, "OPC UA Client reset");
}

void Client::connect(const std::string& server_uri)
{
    GATEWAYLog_local(
            &DDSOPCUA_LOG_ANY_ss,
            "Client connecting to ",
            server_uri.c_str());

    UA_StatusCode retcode = UA_STATUSCODE_GOOD;
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);
        retcode = UA_Client_connect(client_, server_uri.c_str());
    }
    if (retcode != UA_STATUSCODE_GOOD) {
        RTI_THROW_GATEWAY_EXCEPTION_W_ERROR_CODE(
                static_cast<int32_t>(retcode),
                &DDSOPCUA_LOG_OPCUA_CONNECT_SERVER_FAILED_s,
                server_uri.c_str());
    }

    GATEWAYLog_local(
            &DDSOPCUA_LOG_ANY_ss,
            "Client connected to ",
            server_uri.c_str());
}

void Client::disconnect()
{
    if (client_ == nullptr) {
        return;
    }

    GATEWAYLog_local(&DDSOPCUA_LOG_ANY_s, "Disconnecting client");

    UA_StatusCode retcode = UA_STATUSCODE_GOOD;
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);

        retcode = UA_Client_disconnect(client_);
        if (retcode != UA_STATUSCODE_GOOD) {
            GATEWAYLog_exception(
                    &DDSOPCUA_LOG_ANY_s,
                    UA_StatusCode_name(retcode));
        }
    }

    GATEWAYLog_local(&DDSOPCUA_LOG_ANY_s, "Client disconnected");
}

void Client::read_value_attribute(
        sdk::types::Variant& value,
        const sdk::types::NodeId& node_id)
{
    GATEWAYLog_local(
            &DDSOPCUA_LOG_ANY_ss,
            "Reading value attribute on node ",
            node_identifier_to_string(node_id).c_str());

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);
        retval = UA_Client_readValueAttribute(
                client_,
                *(node_id.get_const_ref()),
                value.get());
    }
    if (retval != UA_STATUSCODE_GOOD) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_XML_READ_ATTRIBUTE_suss,
                "value",
                node_id.get_const_ref()->namespaceIndex,
                node_identifier_to_string(node_id).c_str(),
                UA_StatusCode_name(retval));
    }

    GATEWAYLog_local(
            &DDSOPCUA_LOG_ANY_ss,
            "Read value attribute on node ",
            node_identifier_to_string(node_id).c_str());
}

void Client::write_value_attribute(
        const sdk::types::NodeId& node_id,
        const sdk::types::Variant& value)
{
    GATEWAYLog_local(
            &DDSOPCUA_LOG_ANY_ss,
            "Writing value attribute on node ",
            node_identifier_to_string(node_id).c_str());

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);
        retval = UA_Client_writeValueAttribute(
                client_,
                *(node_id.get_const_ref()),
                value.get_const_ref());
    }
    if (retval != UA_STATUSCODE_GOOD) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_XML_WRITE_ATTRIBUTE_suss,
                "value",
                node_id.get_const_ref()->namespaceIndex,
                node_identifier_to_string(node_id).c_str(),
                UA_StatusCode_name(retval));
    }

    GATEWAYLog_local(
            &DDSOPCUA_LOG_ANY_ss,
            "Wrote value attribute on node ",
            node_identifier_to_string(node_id).c_str());
}

sdk::types::BuiltinTypeKind Client::read_value_attribute_type_kind(
        const sdk::types::NodeId& node_id)
{
    sdk::types::Variant variant;
    read_value_attribute(variant, node_id);
    return variant.get_type_kind();
}

uint32_t Client::create_subscription(const SubscriptionProperty& property)
{
    // Configure subscription starting from the default settings
    UA_CreateSubscriptionRequest request =
            UA_CreateSubscriptionRequest_default();
    request.requestedPublishingInterval =
            property.requested_publishing_interval();
    request.requestedMaxKeepAliveCount =
            property.requested_max_keep_alive_count();
    request.maxNotificationsPerPublish =
            property.max_notifications_per_publish();
    request.requestedLifetimeCount = property.requested_lifetime_count();
    request.priority = property.priority();

    GATEWAYLog_local(
            &DDSOPCUA_LOG_CREATE_SUBSCRIPTION_uduuud,
            request.maxNotificationsPerPublish,
            request.requestedPublishingInterval,
            request.requestedMaxKeepAliveCount,
            request.maxNotificationsPerPublish,
            request.requestedLifetimeCount,
            request.priority);

    client_mutex_.take();
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(
            client_,   // client
            request,   // subscriptionRequest
            nullptr,   // subscriptionContext
            nullptr,   // statusChangeCallback
            nullptr);  // deleteCallback

    client_mutex_.give();
    if (response.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_OPCUA_CREATE_SUBSCRIPTION_FAILED_u,
                response.subscriptionId);
    }

    GATEWAYLog_local(
            &DDSOPCUA_LOG_CREATED_SUBSCRIPTION_ufuu,
            response.subscriptionId,
            response.revisedPublishingInterval,
            response.revisedMaxKeepAliveCount,
            response.revisedLifetimeCount);

    return response.subscriptionId;
}

void Client::delete_subscription(const uint32_t& subscription_id)
{
    UA_StatusCode status_code = UA_STATUSCODE_GOOD;
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);

        status_code =
                UA_Client_Subscriptions_deleteSingle(client_, subscription_id);
    }
    if (status_code != UA_STATUSCODE_GOOD) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_OPCUA_DELETE_SUBSCRIPTION_FAILED_u,
                subscription_id);
    }
}

uint32_t Client::add_monitored_item(
        const uint32_t& subscription_id,
        const MonitoredNodeAttribute& monitored_item,
        DataChangeNotificationFnc monitoreditem_handling_fnc,
        void* monitoreditem_context)
{
    UA_MonitoredItemCreateRequest req = UA_MonitoredItemCreateRequest_default(
            *(monitored_item.node_id().get_const_ref()));

    const MonitoringProperty& monitoring_properties =
            monitored_item.monitoring_properties();
    req.requestedParameters.samplingInterval =
            monitoring_properties.sampling_interval();
    req.requestedParameters.queueSize = monitoring_properties.queue_size();
    req.requestedParameters.discardOldest =
            monitoring_properties.discard_oldest();

    GATEWAYLog_local(
            &DDSOPCUA_LOG_ADD_MONITORED_ITEM_sfud,
            node_identifier_to_string(monitored_item.node_id()).c_str(),
            req.requestedParameters.samplingInterval,
            req.requestedParameters.queueSize,
            req.requestedParameters.discardOldest);

    client_mutex_.take();
    UA_MonitoredItemCreateResult resp =
            UA_Client_MonitoredItems_createDataChange(
                    client_,                     // client
                    subscription_id,             // subscriptionId
                    UA_TIMESTAMPSTORETURN_BOTH,  // timestampsToReturn
                    req,                         // createRequest
                    monitoreditem_context,       // context
                    monitoreditem_handling_fnc,  // dataChangeCallback
                    nullptr);                    // deleteCallback
    client_mutex_.give();
    if (resp.statusCode != UA_STATUSCODE_GOOD) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_OPCUA_ADD_MONITORED_ITEM_FAILED_ss,
                monitored_item.name().c_str(),
                UA_StatusCode_name(resp.statusCode));
    }

    GATEWAYLog_local(
            &DDSOPCUA_LOG_ADDED_MONITORED_ITEM_ufu,
            resp.monitoredItemId,
            resp.revisedSamplingInterval,
            resp.revisedQueueSize);

    return resp.monitoredItemId;
}

bool Client::delete_monitored_item(
        const uint32_t subscription_id,
        const uint32_t monitored_item_id)
{
    UA_StatusCode retcode = UA_STATUSCODE_GOOD;
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);
        retcode = UA_Client_MonitoredItems_deleteSingle(
                client_,
                subscription_id,
                monitored_item_id);
    }

    return retcode == UA_STATUSCODE_GOOD ? true : false;
}

void Client::run_iterate(uint16_t timeout)
{
    UA_StatusCode retcode = UA_STATUSCODE_GOOD;
    {
        rti::core::SemaphoreGuard mutex_guard(client_mutex_);
        retcode = UA_Client_run_iterate(client_, 0);
    }

    // // The sleep corresponding to the timeout is actually performed by the
    // // wrapper implementation to avoid blocking the OPC UA Client
    rti::util::sleep(dds::core::Duration::from_millisecs(timeout));
}

}}}}  // namespace rti::opcua::sdk::client
