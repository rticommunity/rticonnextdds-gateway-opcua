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

#ifndef RTI_OPCUA_PROCESSORS_OPCUA_PUBLICATION_PROCESSOR_HPP_
#define RTI_OPCUA_PROCESSORS_OPCUA_PUBLICATION_PROCESSOR_HPP_

#include <dds/core/Optional.hpp>
#include <dds/core/xtypes/DynamicData.hpp>

#include <rti/routing/processor/Processor.hpp>
#include <rti/routing/processor/ProcessorPlugin.hpp>

#include "config/XmlEntities.hpp"
#include "opcUaSdk/OpcUaSdkClientMonitoredItemProperty.hpp"

namespace rti { namespace ddsopcua { namespace processors {

/**
 * @brief
 *
 */
class PublicationProcessor : public rti::routing::processor::NoOpProcessor {
public:
    PublicationProcessor(
            const std::map<std::string, std::string>& instance_filters,
            const std::vector<opcua::sdk::client::PublicationNodeAttribute>&
                    nodes);

    ~PublicationProcessor();

    void on_input_enabled(
            rti::routing::processor::Route& route,
            rti::routing::processor::Input& input);

    void on_output_enabled(
            rti::routing::processor::Route& route,
            rti::routing::processor::Output& output);

    void on_data_available(rti::routing::processor::Route& route);

private:
    /**
     * @brief Output DynamicData object.
     * Optional member for deferred initialization: this object can be created
     * only when the output is enabled.
     */
    dds::core::optional<dds::core::xtypes::DynamicData> input_data_;
    dds::core::optional<dds::core::xtypes::DynamicData> request_sample_;
    std::map<std::string, std::string> sample_selector_filters_;
    std::map<
            std::string,
            std::vector<opcua::sdk::client::PublicationNodeAttribute>>
            sample_selector_nodes_;
};

class PublicationProcessorPluginProperty {
public:
    PublicationProcessorPluginProperty(RTIXMLUTILSObject* xml_root)
            : xml_root_(xml_root)
    {
    }

    RTIXMLUTILSObject* xml_root() const
    {
        return xml_root_;
    }

private:
    RTIXMLUTILSObject* xml_root_;
};

class PublicationProcessorPlugin
        : public rti::routing::processor::ProcessorPlugin {
public:
    PublicationProcessorPlugin(
            const PublicationProcessorPluginProperty& properties);

    rti::routing::processor::Processor* create_processor(
            rti::routing::processor::Route& route,
            const rti::routing::PropertySet& properties);

    void delete_processor(
            rti::routing::processor::Route& route,
            rti::routing::processor::Processor* processor);

private:
    PublicationProcessorPluginProperty processor_properties_;
};

}}}  // namespace rti::ddsopcua::processors

#endif  // RTI_OPCUA_PROCESSORS_OPCUA_PUBLICATION_PROCESSOR_HPP_
