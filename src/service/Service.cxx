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

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>
#include <rti/ddsopcua/log/LogConfig.hpp>

#include "log/LogMsg.hpp"
#include "plugins/adapters/DdsOpcUaAdapter.hpp"
#include "plugins/adapters/DdsOpcUaAdapterProperty.hpp"
#include "plugins/processors/OpcUaAttributeServiceProcessor.hpp"
#include "plugins/processors/OpcUaPublicationProcessor.hpp"

#include "RtiRoutingServiceImplDecl.hpp"
#include "Service.hpp"

namespace rti { namespace ddsopcua { namespace service {

RTI_RoutingServiceProductInfo DDSOPCUA_PRODUCT_INFO = {
    (char*) SERVICE_NAME,              // executable name
    nullptr,                           // shell executable name
    (char*) "RTI OPC UA/DDS Gateway",  // product name
    (char*) "ddsopcua",                // log product name
    (char*) "RTI_OPCUA_DDS_Gateway"    // default app name
};

ServiceGlobalsGuard::ServiceGlobalsGuard()
{
    Service::initialize_globals();
}

ServiceGlobalsGuard::~ServiceGlobalsGuard()
{
    Service::finalize_globals();
}

Service::Service(
        const GatewayProperty& property,
        const RTI_RoutingServiceRemoteShutdownHook* shutdown_hook)
        : property_(property),
          xml_support_(property),
          routing_service_property_(),
          routing_service_(dds::core::null)

{
    if (!property_.ignore_default_files()) {
        xml_support_.load_default_files();
    }

    if (!property_.cfg_file().empty()) {
        xml_support_.parse_file(property_.cfg_file());
    }

    if (property_.service_name().empty()) {
        print_available_configurations();
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_ANY_FAILURE_s,
                "The configuration name is required. Use '-cfgName <name>'"
                "to specify a configuration name.");
    }

    initialize_router_property(routing_service_property_);

    routing_service_ = rti::routing::RoutingService(routing_service_property_);
}

Service::~Service()
{
    try {
        stop();
    } catch (std::exception& e) {
        GATEWAYLog_exception(&DDSOPCUA_LOG_ANY_s, e.what());
    } catch (...) {
        GATEWAYLog_exception(&DDSOPCUA_LOG_UNEXPECTED_EXCEPTION);
    }
}

void Service::initialize_globals()
{
#if RTI_DDS_VERSION_MAJOR >= 7
    bool rs_glob_initialized = RTI_RoutingService_initialize_globalsI();
#else
    bool rs_glob_initialized = RTI_RoutingService_initialize_globals();
#endif
    if (!rs_glob_initialized) {
            RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_INIT_FAILURE_s,
                "routing service globals");
    }

    RTI_RoutingService_reset_product_info();
    RTI_RoutingService_set_product_info(&DDSOPCUA_PRODUCT_INFO);
    rti::ddsopcua::log::LogConfig::instance();
    rti::ddsopcua::config::XmlSupport::initialize_globals();
}

void Service::finalize_globals()
{
#if RTI_DDS_VERSION_MAJOR >= 7
    RTI_RoutingService_finalize_globalsI();
#else
    RTI_RoutingService_finalize_globals();
#endif
    rti::ddsopcua::config::XmlSupport::finalize_globals();
}

void Service::initialize_router_property(
        rti::routing::ServiceProperty& property)
{
    property.service_name(property_.service_name());
    property.application_name(SERVICE_NAME);
    property.enforce_xsd_validation(false);
    property.domain_id_base(0);
    property.enable_administration(false);
    property.enable_monitoring(false);
    property.skip_default_files(false);

    std::vector<std::string> config_strings;
    config::XmlSupport::convert_xml_to_configuration_strings(
            config_strings,
            xml_support_.to_router_configuration().get());
    property.cfg_strings(config_strings);
}

void Service::print_available_configurations()
{
    xml_support_.print_available_configurations();
}


void Service::start()
{
    adapters::DdsOpcUaAdapterProperty adapter_property(
            xml_support_.xml_root(),
            property_.service_name());

    routing_service_.attach_adapter_plugin(
            new adapters::DdsOpcUaAdapter(adapter_property),
            SERVICE_PLUGIN_NAME);

    routing_service_.attach_processor_plugin(
            new processors::AttributeServiceSetProcessorPlugin(),
            ATTRIBUTE_SERVICE_SET_PROCESSOR_NAME);

    routing_service_.attach_processor_plugin(
            new processors::PublicationProcessorPlugin(xml_support_.xml_root()),
            PUBLICATION_PROCESSOR_NAME);

    routing_service_.start();
}

void Service::stop()
{
    routing_service_.stop();
}

}}}  // namespace rti::ddsopcua::service
