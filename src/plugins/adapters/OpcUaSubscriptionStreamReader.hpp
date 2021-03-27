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

#ifndef RTI_OPCUA_ADAPTER_OPCUASUBSCRIPTIONSTREAMREADER_HPP_
#define RTI_OPCUA_ADAPTER_OPCUASUBSCRIPTIONSTREAMREADER_HPP_

#include <map>
#include <string>
#include <vector>

#include <dds/core/xtypes/DynamicData.hpp>
#include <rti/routing/adapter/AdapterPlugin.hpp>

#include "plugins/adapters/DdsOpcUaAdapterProperty.hpp"
#include "plugins/adapters/OpcUaPublishRequestThread.hpp"
#include "opcUaSdk/OpcUaSdkClient.hpp"
#include "service/Utils.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

class OpcUaSubscriptionStreamReader
        : public rti::routing::adapter::DynamicDataStreamReader {
public:
    typedef std::function<void(
            dds::core::xtypes::DynamicData& dds_data,
            const UA_Variant& opcua_data,
            const std::string& dds_member_name)>
            OpcUa2DdsConversionFunction;

    OpcUaSubscriptionStreamReader(
            const DdsOpcUaAdapterProperty& adapter_property,
            const rti::routing::StreamInfo& stream_info,
            const rti::routing::PropertySet& stream_reader_property,
            rti::routing::adapter::StreamReaderListener* listener,
            opcua::sdk::client::Client& client);

    ~OpcUaSubscriptionStreamReader();

    void initialize_subscription();

    void finalize_subscription();

    void take(
            std::vector<dds::core::xtypes::DynamicData*>& sample_seq,
            std::vector<dds::sub::SampleInfo*>& info_seq) RTI_OVERRIDE;

    void return_loan(
            std::vector<dds::core::xtypes::DynamicData*>& sample_seq,
            std::vector<dds::sub::SampleInfo*>& info_seq) RTI_OVERRIDE;

private:
    rti::core::Semaphore& mutex()
    {
        return mutex_;
    }


    static void on_data_available(
            UA_Client* client,
            UA_UInt32 subscription_id,
            void* subscription_context,
            UA_UInt32 item_id,
            void* item_context,
            UA_DataValue* item_data);

private:
    struct MonitoredItemConversionInfo {
        std::string monitored_item_name;
        OpcUa2DdsConversionFunction conversion_fnc;
        uint32_t subscription_id;
    };

private:
    rti::routing::adapter::StreamReaderListener* stream_reader_listener_;
    rti::opcua::sdk::client::Client& opcua_client_;
    rti::core::Semaphore mutex_;
    std::map<uint32_t, MonitoredItemConversionInfo> monitored_item_id_map_;
    std::vector<dds::core::xtypes::DynamicData> data_samples_;
    std::vector<dds::sub::SampleInfo> info_samples_;
    static const uint32_t MAX_SAMPLES = 1;
    bool monitoring_;
    opcua::sdk::client::SubscriptionProperty subscription_property_;
    std::vector<opcua::sdk::client::MonitoredNodeAttribute> node_attributes_;
};

}}}  // namespace rti::ddsopcua::adapters

#endif  // RTI_OPCUA_ADAPTER_OPCUASUBSCRIPTIONSTREAMREADER_HPP_
