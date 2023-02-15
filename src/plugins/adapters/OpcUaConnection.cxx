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

#include "config/XmlEntities.hpp"
#include "config/XmlTransformationParams.hpp"
#include "plugins/adapters/DdsOpcUaAdapterProperty.hpp"
#include "plugins/adapters/OpcUaAttributeServiceStreamWriter.hpp"
#include "plugins/adapters/OpcUaSubscriptionStreamReader.hpp"
#include "plugins/adapters/OpcUaConnection.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

OpcUaConnection::OpcUaConnection(
        const DdsOpcUaAdapterProperty& adapter_property,
        const rti::routing::PropertySet& connection_property)
        : adapter_property_(adapter_property),
          connection_property_(connection_property),
          opcua_attributeservice_streamreader_(nullptr),
          opcua_client_()
{
    // Fully-qualified name of the opcua server property associated with the
    // connection
    std::string opcua_server_xml_fqn = connection_property.at(
            config::XmlTransformationParams ::
                    DDSOPCUA_OPCUA_CONNECTION_FQN_PROPERTY);

    // Configure new OPC UA Client
    opcua::sdk::client::ClientProperty client_property;
    config::XmlOpcUaClient::get_client_property(
            client_property,
            adapter_property_.xml_root(),
            opcua_server_xml_fqn);
    opcua_client_.reset(client_property);
    run_async_timeout_ = client_property.run_async_timeout;

    // Connect to new OPC UA Client
    std::string server_uri = connection_property_.at(
            config::XmlTransformationParams ::
                    DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY);
    opcua_client_.connect(server_uri);
    opcua_client_connected_ = true;
    opcua_client_async_thread_ = std::thread(
            run_opcua_client,
            std::ref(opcua_client_),
            std::ref(opcua_client_connected_),
            run_async_timeout_);
}

OpcUaConnection::~OpcUaConnection()
{
    opcua_client_.disconnect();
    opcua_client_connected_ = false;
    if (opcua_client_async_thread_.joinable()) {
        opcua_client_async_thread_.join();
    }
}

rti::routing::adapter::StreamWriter* OpcUaConnection::create_stream_writer(
        rti::routing::adapter::Session* session,
        const rti::routing::StreamInfo& stream_info,
        const rti::routing::PropertySet& property)
{
    rti::routing::adapter::StreamWriter* stream_writer = nullptr;

    if (stream_info.stream_name() == "OpcUaAttributeServiceSet_Request") {
        stream_writer = new OpcUaAttributeServiceStreamWriter(
                opcua_client_,
                opcua_attributeservice_streamreader_);
    }

    return stream_writer;
}

void OpcUaConnection::delete_stream_writer(
        rti::routing::adapter::StreamWriter* stream_writer)
{
    delete stream_writer;
}

rti::routing::adapter::StreamReader* OpcUaConnection::create_stream_reader(
        rti::routing::adapter::Session*,
        const rti::routing::StreamInfo& stream_info,
        const rti::routing::PropertySet& stream_reader_property,
        rti::routing::adapter::StreamReaderListener* listener)
{
    rti::routing::adapter::StreamReader* stream_reader = nullptr;

    if (stream_info.stream_name() == "OpcUaAttributeServiceSet_Reply") {
        stream_reader = opcua_attributeservice_streamreader_ =
                new OpcUaAttributeServiceStreamReader(
                        stream_info,
                        opcua_client_);
    } else {
        OpcUaSubscriptionStreamReader* opcua_subs_sr = nullptr;
        try {
            opcua_subs_sr = new OpcUaSubscriptionStreamReader(
                    adapter_property_,
                    stream_info,
                    stream_reader_property,
                    listener,
                    opcua_client_);
            opcua_subs_sr->initialize_subscription();
        } catch (const std::exception& e) {
            GATEWAYLog_exception(&DDSOPCUA_LOG_ANY_s, e.what());
            if (opcua_subs_sr != nullptr) {
                opcua_subs_sr->finalize_subscription();
            }
            return nullptr;
        }

        stream_reader = opcua_subs_sr;
    }

    return stream_reader;
}

void OpcUaConnection::delete_stream_reader(
        rti::routing::adapter::StreamReader* stream_reader)
{
    delete stream_reader;
}

rti::opcua::sdk::client::Client& OpcUaConnection::connection_client()
{
    return opcua_client_;
}

void OpcUaConnection::run_opcua_client(
            opcua::sdk::client::Client& opcua_client,
            bool& client_connected,
            const uint16_t timeout)
{
    while (client_connected) {
        opcua_client.run_iterate(timeout);
    }
}

}}}  // namespace rti::ddsopcua::adapters
