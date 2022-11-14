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

#ifndef RTI_OPCUA_ADAPTER_OPCUACONNECTION_HPP_
#define RTI_OPCUA_ADAPTER_OPCUACONNECTION_HPP_

#include <cstdint>
#include <thread>

#include <rti/routing/PropertySet.hpp>
#include <rti/routing/adapter/Connection.hpp>

#include "plugins/adapters/DdsOpcUaAdapterProperty.hpp"
#include "plugins/adapters/OpcUaAttributeServiceStreamReader.hpp"
#include "opcUaSdk/OpcUaSdkClient.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

class OpcUaConnection : public rti::routing::adapter::Connection {
public:
    OpcUaConnection(
            const DdsOpcUaAdapterProperty& adapter_property,
            const rti::routing::PropertySet& connection_property);

    virtual ~OpcUaConnection();

    rti::routing::adapter::StreamWriter* create_stream_writer(
            rti::routing::adapter::Session* session,
            const rti::routing::StreamInfo& stream_info,
            const rti::routing::PropertySet& properties);

    void delete_stream_writer(rti::routing::adapter::StreamWriter* writer);

    rti::routing::adapter::StreamReader* create_stream_reader(
            rti::routing::adapter::Session* session,
            const rti::routing::StreamInfo& stream_info,
            const rti::routing::PropertySet& properties,
            rti::routing::adapter::StreamReaderListener* listener);

    void delete_stream_reader(rti::routing::adapter::StreamReader* reader);

    opcua::sdk::client::Client& connection_client();

private:
    static void run_opcua_client(
            opcua::sdk::client::Client& client,
            const std::vector<uintptr_t>& async_stream_readers,
            const uint16_t timeout);

private:
    const DdsOpcUaAdapterProperty& adapter_property_;
    const rti::routing::PropertySet& connection_property_;
    std::vector<uintptr_t> async_stream_readers_;
    opcua::sdk::client::Client opcua_client_;
    OpcUaAttributeServiceStreamReader* opcua_attributeservice_streamreader_;
    std::thread opcua_client_async_thread_;
    uint16_t run_async_timeout_;
};

}}}  // namespace rti::ddsopcua::adapters

#endif  // RTI_OPCUA_ADAPTER_OPCUACONNECTION_HPP_
