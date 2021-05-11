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

#include <string>

#include <osapi/osapi_utility.h>
#include <rti/config/Logger.hpp>
#include <rti/core/Exception.hpp>
#include <rti/ddsopcua/log/LogConfig.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>

namespace rti { namespace ddsopcua {

GatewayProperty::GatewayProperty()
        : cfg_file_(""),
          service_name_(""),
          executable_path_(""),
          enforce_xsd_validation_(true),
          ignore_default_files_(false)
{
}

GatewayProperty::GatewayProperty(
        const std::string& service_name,
        const std::string& cfg_file)
        : cfg_file_(cfg_file),
          service_name_(service_name),
          executable_path_(""),
          enforce_xsd_validation_(true),
          ignore_default_files_(false)
{
}


GatewayProperty& GatewayProperty::cfg_file(const std::string& file)
{
    cfg_file_ = file;
    return *this;
}

std::string GatewayProperty::cfg_file() const
{
    return cfg_file_;
}

std::string GatewayProperty::service_name() const
{
    return service_name_;
}

GatewayProperty& GatewayProperty::service_name(const std::string& service_name)
{
    service_name_ = service_name;
    return *this;
}

bool GatewayProperty::enforce_xsd_validation() const
{
    return enforce_xsd_validation_;
}

GatewayProperty& GatewayProperty::enforce_xsd_validation(bool enforce)
{
    enforce_xsd_validation_ = enforce;

    return *this;
}

GatewayProperty& GatewayProperty::ignore_default_files(bool ignore)
{
    ignore_default_files_ = ignore;
    return *this;
}

bool GatewayProperty::ignore_default_files() const
{
    return ignore_default_files_;
}


const std::map<std::string, std::string>& GatewayProperty::user_environment()
        const
{
    return user_env_;
}

std::map<std::string, std::string>& GatewayProperty::user_environment()
{
    return user_env_;
}

GatewayProperty& GatewayProperty::user_environment(
        const std::map<std::string, std::string>& user_environment)
{
    user_env_ = user_environment;

    return *this;
}

}}  // namespace rti::ddsopcua
