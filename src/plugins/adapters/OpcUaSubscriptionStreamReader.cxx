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

#include <vector>

#include <dds/core/Duration.hpp>
#include <dds_c/dds_c_infrastructure.h>
#include <open62541/types.h>

#include "config/XmlEntities.hpp"
#include "config/XmlTransformationParams.hpp"
#include "conversion/OpcUa2DdsDynamicDataConversions.hpp"
#include "log/LogMsg.hpp"
#include "opcUaSdk/OpcUaSdkClient.hpp"
#include "opcUaSdk/OpcUaSdkClientProperty.hpp"

#include "OpcUaSubscriptionStreamReader.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

OpcUaSubscriptionStreamReader::OpcUaSubscriptionStreamReader(
        const DdsOpcUaAdapterProperty& adapter_property,
        const rti::routing::StreamInfo& stream_info,
        const rti::routing::PropertySet& stream_reader_property,
        rti::routing::adapter::StreamReaderListener* listener,
        opcua::sdk::client::Client& client)
        : stream_reader_listener_(listener),
          opcua_client_(client),
          mutex_(RTI_OSAPI_SEMAPHORE_KIND_MUTEX),
          data_samples_(),
          monitoring_(false)
{
    data_samples_.resize(
            MAX_SAMPLES,
            rti::core::native_conversions::cast_from_native<
                    dds::core::xtypes::DynamicType>(*static_cast<DDS_TypeCode*>(
                    stream_info.type_info().type_representation())));
    info_samples_.resize(MAX_SAMPLES);

    // Fully-qualified name of the opcua2dds route property associated with
    // the stream reader
    std::string opcua_subscription_xml_fqn = stream_reader_property.at(
            config::XmlTransformationParams::
                    DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY);

    // Configure new subscription
    config::XmlOpcUaSubscription::get_subscription_property(
            subscription_property_,
            adapter_property.xml_root(),
            opcua_subscription_xml_fqn);

    config::XmlOpcUaEndpoint::get_subscription_node_attribute_property(
            node_attributes_,
            adapter_property.xml_root(),
            opcua_subscription_xml_fqn);
}

void OpcUaSubscriptionStreamReader::initialize_subscription()
{
    uint32_t subscription_id =
            opcua_client_.create_subscription(subscription_property_);

    for (auto monitored_item : node_attributes_) {
        // Read Value attribute beforehand
        rti::opcua::sdk::types::Variant variant;
        opcua_client_.read_value_attribute(variant, monitored_item.node_id());

        dds::core::xtypes::TypeKind dds_type_kind =
                dds::core::xtypes::TypeKind::NO_TYPE;
        // And populate the sample so that we can test if it's assignable
        if (variant.is_scalar()) {
            dds_type_kind = data_samples_.at(0)
                                    .member_info(monitored_item.name())
                                    .member_kind();
            conversion::opc_ua_scalar_variant_to_dds_dynamic_data_fnc(
                    variant.get_type_kind(),
                    dds_type_kind)(
                    data_samples_.at(0),
                    *variant.get(),
                    monitored_item.name());
        } else {
            dds_type_kind = data_samples_.at(0)
                                    .member_info(monitored_item.name())
                                    .element_kind();
            conversion::opc_ua_array_variant_to_dds_dynamic_data_fnc(
                    variant.get_type_kind(),
                    dds_type_kind)(
                    data_samples_.at(0),
                    *variant.get(),
                    monitored_item.name());
        }

        // Add monitored item
        uint32_t monitored_item_id = opcua_client_.add_monitored_item(
                subscription_id,
                monitored_item,
                this->on_data_available,
                static_cast<void*>(this));
        // Save reference to monitored item id in a map
        monitored_item_id_map_[monitored_item_id] = {
            monitored_item.name(),
            variant.is_scalar()
                    ? conversion::opc_ua_scalar_variant_to_dds_dynamic_data_fnc(
                            variant.get_type_kind(),
                            dds_type_kind)
                    : conversion::opc_ua_array_variant_to_dds_dynamic_data_fnc(
                            variant.get_type_kind(),
                            dds_type_kind),
            subscription_id
        };
    }

    monitoring_ = true;
}

void OpcUaSubscriptionStreamReader::finalize_subscription()
{
    // Remove monitored items before closing to avoid calls to
    // on_data_available from occurring while wrapping up
    for (auto monitored_item : monitored_item_id_map_) {
        opcua_client_.delete_monitored_item(
                monitored_item.second.subscription_id,
                monitored_item.first);
    }
}

OpcUaSubscriptionStreamReader::~OpcUaSubscriptionStreamReader()
{
    finalize_subscription();
}

void OpcUaSubscriptionStreamReader::take(
        std::vector<dds::core::xtypes::DynamicData*>& sample_seq,
        std::vector<dds::sub::SampleInfo*>& info_seq)
{
    mutex_.take();

    sample_seq.resize(data_samples_.size());
    info_seq.resize(info_samples_.size());

    sample_seq[0] =
            static_cast<dds::core::xtypes::DynamicData*>(&data_samples_[0]);
    info_seq[0] = static_cast<dds::sub::SampleInfo*>(&info_samples_[0]);
}

void OpcUaSubscriptionStreamReader::return_loan(
        std::vector<dds::core::xtypes::DynamicData*>&,
        std::vector<dds::sub::SampleInfo*>&)
{
    mutex_.give();
}

void OpcUaSubscriptionStreamReader::on_data_available(
        UA_Client*,
        UA_UInt32 subscription_id,
        void*,
        UA_UInt32 item_id,
        void* item_context,
        UA_DataValue* item_data)
{
    if (item_data == nullptr) {
        GATEWAYLog_exception(
                &DDSOPCUA_LOG_ANY_s,
                "on_data_available method received null item_data");
        return;
    }

    OpcUaSubscriptionStreamReader* sr =
            static_cast<OpcUaSubscriptionStreamReader*>(item_context);
    if (sr == nullptr || !sr->monitoring_) {
        return;
    }

    GATEWAYLog_local(
            &DDSOPCUA_LOG_PROCESS_MONITORED_ITEM_uu,
            subscription_id,
            item_id);

    // Critical section
    try {
        rti::core::SemaphoreGuard mutex_guard(sr->mutex_);

        // Get member index here.
        const std::string& member_name =
                sr->monitored_item_id_map_[item_id].monitored_item_name;

        // Convert Data
        sr->monitored_item_id_map_[item_id].conversion_fnc(
                sr->data_samples_.at(0),
                item_data->value,
                member_name);

        // Convert source timestamp
        conversion::opc_ua_date_time_to_dds_time_t(
                sr->info_samples_.at(0)->native().source_timestamp,
                item_data->sourceTimestamp);
    } catch (const std::exception& exception) {
        // TODO: We must propagate this to the service
        GATEWAYLog_exception(&DDSOPCUA_LOG_ANY_FAILURE_s, exception.what());
        return;
    }

    GATEWAYLog_local(
            &DDSOPCUA_LOG_ANY_s,
            "Notifying stream reader of data available");

    // Notify the StreamReader's listener of the new available data
    sr->stream_reader_listener_->on_data_available(sr);
}

}}}  // namespace rti::ddsopcua::adapters
