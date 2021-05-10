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

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include "config/XmlEntities.hpp"
#include "config/XmlTransformationParams.hpp"
#include "conversion/DdsDynamicData2OpcUaConversions.hpp"
#include "conversion/OpcUa2DdsDynamicDataConversions.hpp"
#include "log/LogMsg.hpp"

#include "OpcUaPublicationProcessor.hpp"

namespace rti { namespace ddsopcua { namespace processors {

PublicationProcessor::PublicationProcessor(
        const std::map<std::string, std::string>& sample_selector_filters,
        const std::vector<opcua::sdk::client::PublicationNodeAttribute>&
                node_attributes)
        : sample_selector_filters_(sample_selector_filters),
          sample_selector_nodes_()
{
    // Assign Node Attributes to their corresponding Instance
    for (auto node_attribute : node_attributes) {
        sample_selector_nodes_[node_attribute.publication_properties()
                                       .sample_selector_name()]
                .push_back(node_attribute);
    }

    // Check every Sample Selector with associated Nodes has a registered filter
    for (auto sample_selector_node : sample_selector_nodes_) {
        if (sample_selector_filters_.find(sample_selector_node.first)
            == sample_selector_filters_.end()) {
            RTI_THROW_GATEWAY_EXCEPTION(
                    &DDSOPCUA_LOG_XML_LOOKUP_SELETOR_s,
                    sample_selector_node.first.c_str());
        }
    }
}

PublicationProcessor::~PublicationProcessor()
{
}

void PublicationProcessor::on_input_enabled(
        routing::processor::Route& route,
        routing::processor::Input& input)
{
    if (input.name() == "OpcUaAttributeServiceSet_Reply") {
        return;
    }

    // Create data sample associated with the Input's DDS Topic
    input_data_ = input.get<dds::core::xtypes::DynamicData>().create_data();
}

void PublicationProcessor::on_output_enabled(
        routing::processor::Route& route,
        routing::processor::Output& output)
{
    if (output.name() == "OpcUaAttributeServiceSet_Request") {
        // Create data sample to perform requests on the remote OPC UA Server
        request_sample_ =
                output.get<dds::core::xtypes::DynamicData>().create_data();
    }
}

void PublicationProcessor::on_data_available(routing::processor::Route& route)
{
    using namespace dds::sub;
    using namespace dds::core::xtypes;
    using namespace rti::core::xtypes;

    for (auto input = route.inputs<DynamicData, SampleInfo>().begin();
         input != route.inputs<DynamicData, SampleInfo>().end();
         ++input) {
        if (input->name() == "OpcUaAttributeServiceSet_Reply") {
            // We don't need to process data available in the
            // OpcUaAttributeServiceSet_Reply input
            continue;
        }

        for (auto node : sample_selector_nodes_) {
            routing::processor::LoanedSamples<DynamicData, SampleInfo> samples;
            if (node.first == "*") {
                samples = input->read();
            } else {
                samples = input->select()
                                  .filter(dds::topic::Filter(
                                          sample_selector_filters_[node.first]))
                                  .read();
            }

            for (auto sample : samples) {
                if (!sample->info().valid()) {
                    continue;
                }

                for (auto node_attribute : node.second) {
                    // Set AttributeId
                    request_sample_.get().value<uint32_t>(
                            "request.write_request.attribute_id",
                            node_attribute.attribute_id());

                    // Set NodeId
                    conversion::opc_ua_node_id_to_dds_dynamic_data(
                            request_sample_.get().loan_value(
                                    "request.write_request.node_id"),
                            node_attribute.node_id().get_const_ref());

                    // Set Variant
                    conversion::
                            dds_dynamic_data_member_to_dds_dynamic_data_variant(
                                    request_sample_.get().loan_value(
                                            "request.write_request.value."
                                            "value"),
                                    sample->data(),
                                    node_attribute.publication_properties()
                                            .field_name());

                    route.output<DynamicData>(
                                 "OpcUaAttributeServiceSet_Request")
                            .write(request_sample_.get(), sample->info());
                }
            }
        }

        // Take to remove all samples from the cache and continue
        input->take();
    }
}


PublicationProcessorPlugin::PublicationProcessorPlugin(
        const PublicationProcessorPluginProperty& properties)
        : processor_properties_(properties)
{
}

routing::processor::Processor* PublicationProcessorPlugin::create_processor(
        routing::processor::Route&,
        const routing::PropertySet& properties)
{
    routing::PropertySet::const_iterator it = properties.find(
            config::XmlTransformationParams::
                    DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY);
    if (it == properties.end()) {
        return nullptr;
    }
    std::map<std::string, std::string> sample_selector_filters;
    config::XmlDdsEndpoint::get_sample_selectors(
            sample_selector_filters,
            processor_properties_.xml_root(),
            it->second);

    it = properties.find(
            config::XmlTransformationParams::
                    DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY);
    if (it == properties.end()) {
        return nullptr;
    }
    std::vector<opcua::sdk::client::PublicationNodeAttribute> node_attributes;
    config::XmlOpcUaEndpoint::get_publication_node_attribute_property(
            node_attributes,
            processor_properties_.xml_root(),
            it->second);

    return new PublicationProcessor(sample_selector_filters, node_attributes);
}

void PublicationProcessorPlugin::delete_processor(
        routing::processor::Route&,
        routing::processor::Processor* processor)
{
    delete processor;
}

}}}  // namespace rti::ddsopcua::processors
