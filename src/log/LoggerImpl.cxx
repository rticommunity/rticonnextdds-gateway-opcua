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

#include <rti/ddsopcua/DdsOpcUaGateway.hpp>
#include <rti/ddsopcua/log/LogConfig.hpp>

namespace rti { namespace ddsopcua {

Logger& Logger::instance()
{
    static Logger singleton;
    return singleton;
}

void Logger::verbosity(rti::config::Verbosity verbosity)
{
    // Set service verbosity
    rti::ddsopcua::log::LogConfig::instance().verbosity(verbosity);
}

rti::config::Verbosity Logger::verbosity()
{
    return rti::ddsopcua::log::LogConfig::instance().verbosity();
}

Logger::~Logger()
{
}

Logger::Logger()
{
}


}}  // namespace rti::ddsopcua
