/*
 * (c) 2918-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#ifndef RTI_OPCUA_SERVICE_PARAMETERMANAGER_HPP_
#define RTI_OPCUA_SERVICE_PARAMETERMANAGER_HPP_

#include <string>
#include <vector>

// #include <routingservice/routingservice_service_impl.h>
#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>

namespace rti { namespace ddsopcua { namespace service {

enum ArgumentKind {
    ARGUMENT_KIND_VOID,
    ARGUMENT_KIND_STRING,
    ARGUMENT_KIND_INT
};

struct ArgumentValue {
    int int_value;
    std::string string_value;
};

struct Argument {
    Argument(
            const std::string& arg_name,
            const std::string& arg_description,
            ArgumentKind arg_type,
            bool arg_required,
            bool arg_visible,
            bool arg_exit,
            bool arg_set,
            const ArgumentValue& arg_value = ArgumentValue())
            : name(arg_name),
              description(arg_description),
              type(arg_type),
              required(arg_required),
              visible(arg_visible),
              exit(arg_exit),
              set(arg_set),
              value(arg_value)
    {
    }
    /**
     * @brief Argument name
     */
    std::string name;
    /**
     * @brief Argument description
     */
    std::string description;
    /**
     * @brief Argument kind (e.g., string, integer, or void)
     */
    ArgumentKind type;
    /**
     * @brief Indicates whether the parameter is required
     */
    bool required;
    /**
     * @brief Indicates whether a parameter is public (i.e., visible to the
     * user)
     */
    bool visible;
    /**
     * @brief Indicates whether the parser must exit after parsing this
     * argument
     */
    bool exit;
    /**
     * @brief Indicates whether the argument has been parsed.
     */
    bool set;
    /**
     * @brief Argument value. It can be of different kinds, depending on the
     * ArgumentKind.
     */
    ArgumentValue value;
};

class ParameterManager {
public:
    ParameterManager();
    ~ParameterManager();
    void parse(int argc, char** argv);
    const std::vector<Argument>& arguments() const;
    std::string executable_name() const;

    // private:
    void process_commandline_arguments(int argc, char** argv);

private:
    std::string executable_name_;
    std::vector<Argument> arguments_;
};

}}}  // namespace rti::ddsopcua::service

#endif  // RTI_OPCUA_SERVICE_PARAMETERMANAGER_HPP_
