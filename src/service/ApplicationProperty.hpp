/*
 * (c) 2919-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#ifndef HPP_SERVICE_APPLICATIONPROPERTY_HPP_
#define HPP_SERVICE_APPLICATIONPROPERTY_HPP_

#include <dds/core/SafeEnumeration.hpp>
#include <rti/config/Logger.hpp>
#include <routingservice/routingservice_service.h>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>

#include "RtiRoutingServiceImplDecl.hpp"

namespace rti { namespace ddsopcua { namespace service {

class ApplicationProperty {
public:
    /**
     * @brief Construct a new Application Property object
     * This parses command-line arguments. Parse failures cause exceptions.
     * shutdown_hook is an optional callback installed on the service that will
     * be called back when it is shutting down.
     */
    ApplicationProperty(
            int argc,
            char** argv,
            const RTI_RoutingServiceRemoteShutdownHook& shutdown_hook);

    const std::string& program_name() const;

    const GatewayProperty& gateway_property() const;

    rti::config::Verbosity gateway_verbosity() const;

    rti::config::Verbosity dds_verbosity() const;

    rti::config::PrintFormat log_format() const;

    int32_t max_objects_per_thread() const;

    const RTI_RoutingServiceHeapMonitorProperty& heap_monitor_property() const;

    const RTI_RoutingServiceRemoteShutdownHook& shutdown_hook() const;

    const std::string& help_message() const;

    bool is_help_requested() const;

    bool is_version_requested() const;

    bool is_available_configuration_list_requested() const;

private:
    ApplicationProperty& log_format(const std::string& log_format_str);

    void construct_help_message(
            const std::string& program_name,
            const std::string& service_name,
            const std::string& service_tag,
            int default_verbosity);

private:
    bool is_help_requested_;
    bool is_version_requested_;
    bool is_available_configuration_list_requested_;
    std::string program_name_;
    rti::config::Verbosity gateway_verbosity_;
    rti::config::Verbosity dds_verbosity_;
    rti::config::PrintFormat log_format_;
    int32_t max_objects_per_thread_;
    std::string help_message_;
    std::string version_message_;
    GatewayProperty gateway_property_;
    RTI_RoutingServiceHeapMonitorProperty heap_monitor_property_;
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook_;
};

}}}  // namespace rti::ddsopcua::service

#endif  // HPP_SERVICE_APPLICATIONPROPERTY_HPP_
