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

#ifndef RTI_OPCUASDK_CLIENTPROPERTY_HPP_
#define RTI_OPCUASDK_CLIENTPROPERTY_HPP_

#include <open62541/client.h>
#include <open62541/client_config_default.h>

namespace rti { namespace opcua { namespace sdk { namespace client {

class ClientProperty {
public:
    ClientProperty() : run_async_timeout(1000)
    {
        UA_Client* client = UA_Client_new();
        UA_ClientConfig* configuration = UA_Client_getConfig(client);
        UA_ClientConfig_setDefault(configuration);

        timeout = configuration->timeout;
        secure_channel_lifetime = configuration->secureChannelLifeTime;
        local_connection_protocol_version =
                configuration->localConnectionConfig.protocolVersion;
        local_connection_send_buffer_size =
                configuration->localConnectionConfig.sendBufferSize;
        local_connection_recv_buffer_size =
                configuration->localConnectionConfig.recvBufferSize;
        local_connection_max_message_size =
                configuration->localConnectionConfig.localMaxMessageSize;
        local_connection_max_chunk_count =
                configuration->localConnectionConfig.localMaxChunkCount;
        UA_Client_delete(client);
    }

    void to_native_configuration(UA_ClientConfig& configuration) const
    {
        configuration.timeout = timeout;
        configuration.secureChannelLifeTime = secure_channel_lifetime;
        configuration.localConnectionConfig.protocolVersion =
                local_connection_protocol_version;
        configuration.localConnectionConfig.sendBufferSize =
                local_connection_send_buffer_size;
        configuration.localConnectionConfig.recvBufferSize =
                local_connection_recv_buffer_size;
        configuration.localConnectionConfig.localMaxMessageSize =
                local_connection_max_message_size;
        configuration.localConnectionConfig.localMaxChunkCount =
                local_connection_max_chunk_count;
    }

    uint32_t timeout;
    uint32_t secure_channel_lifetime;
    uint32_t local_connection_protocol_version;
    uint32_t local_connection_send_buffer_size;
    uint32_t local_connection_recv_buffer_size;
    uint32_t local_connection_max_message_size;
    uint32_t local_connection_max_chunk_count;
    uint16_t run_async_timeout;
};

}}}}  // namespace rti::opcua::sdk::client

#endif  // RTI_OPCUASDK_CLIENTPROPERTY_HPP_
