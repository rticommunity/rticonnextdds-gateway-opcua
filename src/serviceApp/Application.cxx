/*
 * (c) 2019-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#include <dds/domain/DomainParticipant.hpp>
#include <rti/config/Version.hpp>
#include <rti/ddsopcua/log/LogConfig.hpp>

#include "config/XmlSupport.hpp"

#include "Application.hpp"

#define RTI_PRODUCT_VERSION_MAJOR RTI_DDS_VERSION_MAJOR
#define RTI_PRODUCT_VERSION_MINOR RTI_DDS_VERSION_MINOR
#define RTI_PRODUCT_VERSION_RELEASE RTI_DDS_VERSION_RELEASE
#define RTI_PRODUCT_VERSION_REVISION RTI_DDS_VERSION_REVISION

namespace rti { namespace ddsopcua {

Application::Application(
        const rti::ddsopcua::service::ApplicationProperty& property)
        : application_property_(property), gateway_(dds::core::null)
{
    rti::ddsopcua::Logger::instance().verbosity(property.gateway_verbosity());
    rti::config::Logger::instance().verbosity(property.dds_verbosity());
    rti::config::Logger::instance().print_format(property.log_format());

    rti::ddsopcua::Gateway::initialize_globals();

    if (property.max_objects_per_thread() > 0) {
        dds::domain::qos::DomainParticipantFactoryQos factory_qos =
                dds::domain::DomainParticipant::participant_factory_qos();
        factory_qos->resource_limits.max_objects_per_thread(
                property.max_objects_per_thread());
        dds::domain::DomainParticipant::participant_factory_qos(factory_qos);
    }

    DDS_ProductVersion_t native_product_version;
    native_product_version.major = RTI_PRODUCT_VERSION_MAJOR;
    native_product_version.minor = RTI_PRODUCT_VERSION_MINOR;
    native_product_version.release = RTI_PRODUCT_VERSION_RELEASE;
    native_product_version.revision = RTI_PRODUCT_VERSION_REVISION;
    app_version_.native(native_product_version);
}

Application::~Application()
{
    stop();
    Gateway::finalize_globals();
    dds::domain::DomainParticipant::finalize_participant_factory();
}


bool Application::start()
{
    if (application_property_.is_help_requested()) {
        print_usage();
        return false;
    } else if (application_property_.is_version_requested()) {
        print_product_and_version();
        return false;
    } else if (application_property_
                       .is_available_configuration_list_requested()) {
        print_available_configurations();
        return false;
    }

    RTILogParamString_printPlain(
            "%s %s\n",
            SERVICE_NAME,
            app_version_string().c_str());

    gateway_ = Gateway(new GatewayImpl(
            application_property_.gateway_property(),
            static_cast<const void*>(&application_property_.shutdown_hook())));

    gateway_.start();

    return true;
}

void Application::stop()
{
    if (gateway_ != dds::core::null) {
        gateway_.stop();
        gateway_ = dds::core::null;
    }
}

std::string Application::app_version_string() const
{
    char dst[DDS_PRODUCTVERSION_MAX_STRING_SIZE];
    DDS_ProductVersion_to_string(
                const_cast<DDS_ProductVersion_t*>(&(app_version_.native())),
                dst);

    std::string dst_str(dst);
    if (app_version_.revision_version() == 0) {
        // Remove the revision from the string
        std::string::size_type pos = dst_str.find_last_of('.');
        dst_str.erase(pos);
    }
    return dst_str;
}

void Application::print_usage() const
{
    std::cout << application_property_.help_message();
}

void Application::print_product_and_version() const
{
    std::cout << "RTI OPC UA/DDS Gateway Service " << app_version_string()
              << std::endl;
}

void Application::print_available_configurations() const
{
    config::XmlSupport xml_support(application_property_.gateway_property());
    if (!application_property_.gateway_property().ignore_default_files()) {
        xml_support.load_default_files();
    }

    if (!application_property_.gateway_property().cfg_file().empty()) {
        xml_support.parse_file(
                application_property_.gateway_property().cfg_file());
    }

    xml_support.print_available_configurations();
}

}}  // namespace rti::ddsopcua
