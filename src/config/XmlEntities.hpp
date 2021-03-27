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

#ifndef RTI_DDS_OPCUA_CONFIG_XML_ENTITIES_
#define RTI_DDS_OPCUA_CONFIG_XML_ENTITIES_

#include <string>
#include <map>

#include "opcUaSdk/OpcUaSdkClientProperty.hpp"
#include "opcUaSdk/OpcUaSdkClientSubscriptionProperty.hpp"
#include "opcUaSdk/OpcUaSdkClientMonitoredItemProperty.hpp"

#include "RtiXmlUtilsDecl.hpp"

namespace rti { namespace ddsopcua { namespace config {

class XmlDdsEndpoint {
public:
    static void get_sample_selectors(
            std::map<std::string, std::string>& sample_selectors,
            RTIXMLUTILSObject* xml_root,
            const std::string& dds_input_xml_fqn);
};

class XmlOpcUaClient {
public:
    static void get_client_property(
            opcua::sdk::client::ClientProperty& client_properties,
            RTIXMLUTILSObject* xml_root,
            const std::string& opcua_server_xml_fqn);
};

class XmlOpcUaSubscription {
public:
    static void get_subscription_property(
            opcua::sdk::client::SubscriptionProperty& subscription_properties,
            RTIXMLUTILSObject* xml_root,
            const std::string& opcua_route_xml_fqn);
};

class XmlOpcUaEndpoint {
public:
    static void get_subscription_node_attribute_property(
            std::vector<opcua::sdk::client::MonitoredNodeAttribute>&
                    node_attribute_property,
            RTIXMLUTILSObject* xml_root,
            const std::string& opcua_endpoint_fqn);

    static void get_publication_node_attribute_property(
            std::vector<opcua::sdk::client::PublicationNodeAttribute>&
                    node_attribute_property,
            RTIXMLUTILSObject* xml_root,
            const std::string& opcua_endpoint_fqn);

private:
    static void get_node_attribute_property(
            opcua::sdk::client::NodeAttribute& node_attribute,
            RTIXMLUTILSObject* node_attribute_xml_oject);
};

}}}  // namespace rti::ddsopcua::config

#endif  // RTI_DDS_OPCUA_CONFIG_XML_ENTITIES_
