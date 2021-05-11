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

#ifndef RTI_DDSOPCUA_SERVICE_APPLICATION_HPP_
#define RTI_DDSOPCUA_SERVICE_APPLICATION_HPP_

#include <rti/core/ProductVersion.hpp>

#include <rti/ddsopcua/DdsOpcUaGateway.hpp>

#include "service/ApplicationProperty.hpp"

namespace rti { namespace ddsopcua {

/**
 * Instanties an OPC UA/DDS Gateway.
 */
class Application {
public:
    Application(const rti::ddsopcua::service::ApplicationProperty& property);
    ~Application();

    /**
     * Based on the properties that were passed in, this API may or may not
     * start an underlying service. The return value indicates whether the
     * service was started or not (this throws an exception in case of errors).
     * calling begin twice may call start() on the service twice.
     */
    bool start();

    /**
     * @brief This calls stop on the underlying service, if it exists.
     */
    void stop();


private:
    std::string app_version_string() const;

    void print_usage() const;

    void print_product_and_version() const;

    void print_available_configurations() const;

private:
    rti::ddsopcua::service::ApplicationProperty application_property_;
    rti::ddsopcua::Gateway gateway_;
    rti::core::ProductVersion app_version_;
};

}}  // namespace rti::ddsopcua

#endif  // RTI_DDSOPCUA_SERVICE_APPLICATION_HPP_
