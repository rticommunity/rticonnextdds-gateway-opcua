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

#include <rti/ddsopcua/log/LogConfig.hpp>

#include "ApplicationProperty.hpp"
#include "ParameterManager.hpp"

namespace rti { namespace ddsopcua { namespace service {


ApplicationProperty::ApplicationProperty(
        int argc,
        char** argv,
        const RTI_RoutingServiceRemoteShutdownHook& shutdown_hook)
        : is_help_requested_(false),
          is_version_requested_(false),
          is_available_configuration_list_requested_(false),
          log_format_(rti::config::PrintFormat::DEFAULT),
          max_objects_per_thread_(0),
          help_message_(""),
          version_message_(""),
          heap_monitor_property_(
                  RTI_RoutingServiceHeapMonitorProperty_INITIALIZER),
          shutdown_hook_(shutdown_hook)
{
    dds_verbosity_ = rti::ddsopcua::log::dds_verbosity_from_level(1);
    gateway_verbosity_ = rti::ddsopcua::log::service_verbosity_from_level(1);

    ParameterManager parameter_manager;
    parameter_manager.parse(argc, argv);

    program_name_ = parameter_manager.executable_name();

    for (auto argument : parameter_manager.arguments()) {
        if (!argument.set) {
            continue;
        }

        if (argument.name == "help") {
            is_help_requested_ = true;
            construct_help_message(
                    program_name_,
                    SERVICE_NAME,
                    SERVICE_TAG,
                    SERVICE_DEFAULT_VERBOSITY);
            return;
        } else if (argument.name == "version") {
            is_version_requested_ = true;
            return;
        } else if (argument.name == "listConfig") {
            is_available_configuration_list_requested_ = true;
            return;
        } else if (argument.name == "cfgName") {
            gateway_property_.service_name(argument.value.string_value);
        } else if (argument.name == "cfgFile") {
            gateway_property_.cfg_file(argument.value.string_value);
        } else if (argument.name == "ignoreXsdValidation") {
            gateway_property_.enforce_xsd_validation(false);
        } else if (argument.name == "logFormat") {
            log_format(argument.value.string_value);
        } else if (argument.name == "maxObjectsPerThread") {
            max_objects_per_thread_ = argument.value.int_value;
        } else if (argument.name == "verbosity") {
            dds_verbosity_ = rti::ddsopcua::log::dds_verbosity_from_level(
                    static_cast<int32_t>(argument.value.int_value));
            gateway_verbosity_ =
                    rti::ddsopcua::log::service_verbosity_from_level(
                            static_cast<int32_t>(argument.value.int_value));
        }
    }
}

bool ApplicationProperty::is_help_requested() const
{
    return is_help_requested_;
}

bool ApplicationProperty::is_version_requested() const
{
    return is_version_requested_;
}

bool ApplicationProperty::is_available_configuration_list_requested() const
{
    return is_available_configuration_list_requested_;
}

const std::string& ApplicationProperty::program_name() const
{
    return program_name_;
}

int32_t ApplicationProperty::max_objects_per_thread() const
{
    return max_objects_per_thread_;
}

const GatewayProperty& ApplicationProperty::gateway_property() const
{
    return gateway_property_;
}

rti::config::Verbosity ApplicationProperty::gateway_verbosity() const
{
    return gateway_verbosity_;
}

const RTI_RoutingServiceHeapMonitorProperty& ApplicationProperty::
        heap_monitor_property() const
{
    return heap_monitor_property_;
}

rti::config::Verbosity ApplicationProperty::dds_verbosity() const
{
    return dds_verbosity_;
}

rti::config::PrintFormat ApplicationProperty::log_format() const
{
    return log_format_;
}

ApplicationProperty& ApplicationProperty::log_format(
        const std::string& log_format_str)
{
    rti::config::PrintFormat log_format;
    if (log_format_str == "TIMESTAMPED") {
        log_format = rti::config::PrintFormat::TIMESTAMPED;
    } else if (log_format_str == "VERBOSE") {
        log_format = rti::config::PrintFormat::VERBOSE;
    } else if (log_format_str == "MINIMAL") {
        log_format_ = rti::config::PrintFormat::MINIMAL;
    } else if (log_format_str == "MAXIMAL") {
        log_format_ = rti::config::PrintFormat::MAXIMAL;
    } else {
        log_format_ = rti::config::PrintFormat::DEFAULT;
    }
    return *this;
}

const RTI_RoutingServiceRemoteShutdownHook& ApplicationProperty::shutdown_hook()
        const
{
    return shutdown_hook_;
}


const std::string& ApplicationProperty::help_message() const
{
    return help_message_;
}

void ApplicationProperty::construct_help_message(
        const std::string& program_name,
        const std::string& service_name,
        const std::string& service_tag,
        int default_verbosity)
{
    if (!help_message_.empty()) {
        return;
    }

    std::stringstream stream;

    stream << "Usage: " << program_name << " "
           << "-cfgName <configuration> [options]\n\nOptions:\n"
           << "-cfgFile\t\t<file>\tSpecifies a configuration file to be "
              "loaded.\n\n"
           << "-cfgName\t\t<name>\tSpecifies a configuration name \n"
           << "\t\t\t\t" << service_name << " will look for a matching\n"
           << "\t\t\t\t" << service_tag << " tag in the configuration file\n"
           << "\t\t\t\tspecified via the -cfgFile tparameter\n"
           << "\t\t\t\tand in the default configuration\n"
           << "\t\t\t\tfiles.\n"
           << "\t\t\t\tThis parameter is required.\n\n"
           << "-verbosity\t\t[0-6]\tControls what type of messages are "
           << "logged:\n"
           << "\t\t\t\t* 0 - silent\n"
           << "\t\t\t\t* 1 - exceptions (DDS and service)\n"
           << "\t\t\t\t* 2 - warnings (service)\n"
           << "\t\t\t\t* 3 - information (service)\n"
           << "\t\t\t\t* 4 - warnings (DDS and service)\n"
           << "\t\t\t\t* 5 - tracing (service)\n"
           << "\t\t\t\t* 6 - tracing (DDS and service)\n"
           << "\t\t\t\tEach verbosity level, n, includes all the\n"
           << "\t\t\t\tverbosity level smaller than n.\n"
           << "\t\t\t\tDefault: " << default_verbosity << "\n\n"
           << "-version\t\t\tPrints the " << service_name << " version\n"
           << "\t\t\t\tnumber.\n\n";
    help_message_ = stream.str();
}

}}}  // namespace rti::ddsopcua::service
