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

#ifndef RTI_DDSOPCUA_GATEWAY_PROPERTY_HPP_
#define RTI_DDSOPCUA_GATEWAY_PROPERTY_HPP_

#include <string>

#include <rti/routing/ServiceProperty.hpp>
#include <rti/config/Logger.hpp>

// SERVICE_NAME and SERVICE_VERSION can be defined at build time, which
// makes it easier to cycle the version.

#ifndef SERVICE_NAME
    #define SERVICE_NAME "RTI OPC UA/DDS Gateway"
#endif

#ifndef SERVICE_VERSION
    #define SERVICE_VERSION ""
#endif

namespace rti { namespace ddsopcua {

static const std::string SERVICE_TAG = "ddsopcua_service";
static const std::string SERVICE_PLUGIN_NAME = "ddsopcua_plugin";
static const std::string ATTRIBUTE_SERVICE_SET_PROCESSOR_NAME =
        "ddsopcua_processor_attribute";
static const std::string PUBLICATION_PROCESSOR_NAME =
        "ddsopcua_processor_opcua_publication";
static const int SERVICE_DEFAULT_VERBOSITY = 1;

class GatewayProperty {
public:
    GatewayProperty();

    GatewayProperty(
            const std::string& service_name,
            const std::string& cfg_file = "");

    /**
     * @brief Path to an @product configuration file
     *
     * @default empty string.
     */
    std::string cfg_file() const;

    /**
     * @brief Getter (see setter with the same name)
     */
    GatewayProperty& cfg_file(const std::string& file);

    /**
     * @brief The name of the @product configuration to run.
     *
     * This is the name used to find the service XML tag in the configuration
     * file; the name that will  be used to refer to this execution in remote
     * administration and
     * monitoring.
     *
     * @default empty string.
     */
    GatewayProperty& service_name(const std::string& service_name);

    /**
     * @brief Getter (see setter with the same name)
     */
    std::string service_name() const;

    /**
     * @brief Controls whether the service applies XSD validation to the loaded
     * configuration.
     *
     * @default true
     */
    GatewayProperty& enforce_xsd_validation(bool enforce);

    /**
     * @brief Getter (see setter with the same name)
     */
    bool enforce_xsd_validation() const;

    /**
     * @brief Set it to true to avoid loading the
     * standard files usually loaded by @product.
     *
     * Only the configuration in ServiceProperty::cfg_file or
     * ServiceProperty::cfg_strings will be loaded.
     *
     * @default false
     */
    GatewayProperty& ignore_default_files(bool ignore);

    /**
     * @brief Getter (see setter with the same name)
     */
    bool ignore_default_files() const;

    /**
     * @brief Dictionary of user variables.
     * The dictionary provides a parallel way to expand XML configuration
     * variables in the form $(my_var), when they are not defined in the
     * environment.
     *
     * @default empty
     */
    GatewayProperty& user_environment(
            const std::map<std::string, std::string>& user_environment);

    /**
     * @brief Getter (see setter with the same name)
     */
    const std::map<std::string, std::string>& user_environment() const;

    /**
     * @brief Non-const getter for modification that can be used to add
     * elements individually. (see setter with the same name)
     */
    std::map<std::string, std::string>& user_environment();


private:
    std::string cfg_file_;
    std::string service_name_;
    std::string executable_path_;
    bool enforce_xsd_validation_;
    std::map<std::string, std::string> user_env_;
    bool ignore_default_files_;
    static const std::string SERVICE_PLUGIN_NAME;
};

}}  // namespace rti::ddsopcua

#endif  // RTI_DDSOPCUA_GATEWAY_PROPERTY_HPP_
