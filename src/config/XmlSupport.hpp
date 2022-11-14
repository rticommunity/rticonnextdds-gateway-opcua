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

#ifndef RTI_DDS_OPCUA_CONFIG_XML_SUPPORT_
#define RTI_DDS_OPCUA_CONFIG_XML_SUPPORT_

#include <memory>
#include <string>

#include <dds_c/dds_c_xml.h>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>

#include "RtiXmlUtilsDecl.hpp"

namespace rti { namespace ddsopcua { namespace config {

class XmlSupport {
public:
    /**
     * native_xmlutilsobject is a std::unique_ptr to RTIXMLUTILSObject
     */
    using native_xmlutilsobject =
            std::unique_ptr<RTIXMLUTILSObject, void (*)(RTIXMLUTILSObject*)>;
    /**
     * Constructor of the XmlSupport class
     */
    XmlSupport(
            const rti::ddsopcua::GatewayProperty& properties,
            bool validate_on_parse = true);

    /**
     * Desctructor of the XmlSupport class
     */
    ~XmlSupport();

    static void initialize_globals();

    static void finalize_globals();

    /**
     * Parses, replaces variables, and validates a configuration file
     * @param filename Path to the configuration file to be parsed.
     */
    void parse_file(const std::string& filename);

    void parse_string_array(const char** string_array, int string_array_length);

    void parse_types_string_array(
            const char** string_array,
            int string_array_length);

    native_xmlutilsobject to_router_configuration();

    static void convert_xml_to_configuration_strings(
            std::vector<std::string>& cfg_strings,
            RTIXMLUTILSObject* xml_object);

    bool load_default_files();

    void print_available_configurations();

    RTIXMLUTILSObject* lookup_service(const std::string& service_name);

    RTIXMLUTILSObject* xml_root() const;

    void validate(RTIXMLUTILSObject* xml_object);

    const std::string& service_tag();

    // Static strings for configuration files
    static const std::string& path_to_module_xml();
    static const std::string& path_to_shipped_xml();
    static const std::string& default_configuration_file_name();
    static const std::string& user_configuration_file_name();

private:
    void process_loaded_xml(RTIXMLUTILSObject* xml_object);


private:
    RTIXMLUTILSObject* xml_root_;
    bool validate_on_parse_;
    std::unique_ptr<RTIXMLUTILSValidator, void (*)(RTIXMLUTILSValidator*)>
            validator_;
    std::unique_ptr<RTIXMLUTILSTransformer, void (*)(RTIXMLUTILSTransformer*)>
            transformer_;
    const std::map<std::string, std::string>& user_env_;
};

}}}  // namespace rti::ddsopcua::config

#endif  // RTI_DDS_OPCUA_CONFIG_XML_SUPPORT_
