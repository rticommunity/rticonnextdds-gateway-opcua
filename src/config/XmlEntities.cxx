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

#include <reda/reda_string.h>

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include "opcUaSdk/OpcUaSdkDataTypes.hpp"
#include "log/LogMsg.hpp"

#include "RtiXmlUtilsDecl.hpp"
#include "XmlEntities.hpp"
#include "XmlSupport.hpp"

inline void xml_object_check_lookup(
        struct RTIXMLUTILSObject* object,
        struct RTIXMLUTILSObject* parent_object)
{
    if (object == nullptr) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_XML_LOOKUP_PARENT_s,
                RTIXMLUTILSObject_getFullyQualifiedName(parent_object));
    }
}

inline void xml_object_check_lookup_attribute(
        const char* attribute,
        const char* attribute_name,
        struct RTIXMLUTILSObject* object)
{
    if (attribute == nullptr) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_XML_LOOKUP_ATTRIBUTE_ss,
                attribute_name,
                RTIXMLUTILSObject_getFullyQualifiedName(object));
    }
}

inline void check_type_conversion(
        bool type_conversion_result,
        struct RTIXMLUTILSObject* object)
{
    if (!type_conversion_result) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_XML_CONVERT_OBJECT_s,
                RTIXMLUTILSObject_getFullyQualifiedName(object));
    }
}

inline void check_text(const char* text, struct RTIXMLUTILSObject* object)
{
    if (text == nullptr) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_XML_CONVERT_OBJECT_s,
                RTIXMLUTILSObject_getFullyQualifiedName(object));
    }
}

namespace rti { namespace ddsopcua { namespace config {

const std::string DDS_TOPIC_FIELD_NAME_ATTRIBUTE = "dds_topic_field_name";
const std::string OPCUA_SUBSCRIPTION_TAG_NAME = "opcua_input";
const std::string OPCUA_SUBSCRIPTION_PROTOCOL_TAG_NAME =
        "subscription_protocol";
const std::string OPCUA_MONITORED_ITEMS_TAG_NAME = "monitored_items";
const std::string OPCUA_ATTRIBUTE_ID_TAG_NAME = "attribute_id";
const std::string OPCUA_NODE_ID_TAG_NAME = "node_id";
const std::string OPCUA_NAMESPACE_INDEX_TAG_NAME = "namespace_index";


void XmlDdsEndpoint::get_sample_selectors(
        std::map<std::string, std::string>& sample_selectors,
        struct RTIXMLUTILSObject* xml_root,
        const std::string& dds_input_xml_fqn)
{
    struct RTIXMLUTILSObject* dds_input_xml_object =
            RTIXMLUTILSObject_lookUp(xml_root, dds_input_xml_fqn.c_str());
    xml_object_check_lookup(dds_input_xml_object, xml_root);

    // Always pre-register sample selector "*" with an empty filter
    sample_selectors["*"] = "";

    RTIXMLUTILSObject* sample_selectors_xml_object =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    dds_input_xml_object,
                    "sample_selectors");
    if (sample_selectors_xml_object == nullptr) {
        return;
    }

    RTIXMLUTILSObject* selector_xml_object =
            RTIXMLUTILSObject_getFirstChild(sample_selectors_xml_object);
    while (selector_xml_object != nullptr) {
        const char* selector_name =
                RTIXMLUTILSObject_getAttribute(selector_xml_object, "name");
        xml_object_check_lookup_attribute(
                selector_name,
                "name",
                selector_xml_object);

        RTIXMLUTILSObject* filter_xml_object =
                RTIXMLUTILSObject_getFirstChildWithTag(
                        selector_xml_object,
                        "filter");
        const char* filter = RTIXMLUTILSObject_getText(filter_xml_object);
        check_text(filter, filter_xml_object);

        sample_selectors[selector_name] = filter;

        selector_xml_object =
                RTIXMLUTILSObject_getNextSibling(selector_xml_object);
    }
}

void XmlOpcUaClient::get_client_property(
        opcua::sdk::client::ClientProperty& client_properties,
        struct RTIXMLUTILSObject* xml_root,
        const std::string& opcua_server_fqn)
{
    struct RTIXMLUTILSObject* opcua_server_xml_object =
            RTIXMLUTILSObject_lookUp(xml_root, opcua_server_fqn.c_str());
    xml_object_check_lookup(opcua_server_xml_object, xml_root);

    // Set timeout if specified
    struct RTIXMLUTILSObject* timeout_xml_object =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_server_xml_object,
                    "timeout");
    if (timeout_xml_object != nullptr) {
        uint32_t timeout;
        check_type_conversion(
                static_cast<bool>(REDAString_strToUnsignedLong(
                        RTIXMLUTILSObject_getText(timeout_xml_object),
                        &timeout)),
                timeout_xml_object);

        client_properties.timeout = timeout;
    }
    // Set secure_channel_lifetime if specified
    struct RTIXMLUTILSObject* lifetime_xml_object =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_server_xml_object,
                    "secure_channel_lifetime");
    if (lifetime_xml_object != nullptr) {
        uint32_t secure_channel_lifetime;
        check_type_conversion(
                static_cast<bool>(REDAString_strToUnsignedLong(
                        RTIXMLUTILSObject_getText(lifetime_xml_object),
                        &secure_channel_lifetime)),
                lifetime_xml_object);

        client_properties.secure_channel_lifetime = secure_channel_lifetime;
    }

    // Set timeout if specified
    struct RTIXMLUTILSObject* run_async_timeout =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_server_xml_object,
                    "run_asynchronous_timeout");
    if (run_async_timeout != nullptr) {
        uint16_t timeout;
        check_type_conversion(
                static_cast<uint16_t>(REDAString_strToUnsignedShort(
                        RTIXMLUTILSObject_getText(run_async_timeout),
                        &timeout)),
                run_async_timeout);

        client_properties.run_async_timeout = timeout;
    }
}

void XmlOpcUaSubscription::get_subscription_property(
        opcua::sdk::client::SubscriptionProperty& subscription_properties,
        RTIXMLUTILSObject* xml_root,
        const std::string& opcua_subscription_fqn)
{
    RTIXMLUTILSObject* opcua_subscription_xml =
            RTIXMLUTILSObject_lookUp(xml_root, opcua_subscription_fqn.c_str());
    xml_object_check_lookup(opcua_subscription_xml, xml_root);

    RTIXMLUTILSObject* opcua_subscription_protocol_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_subscription_xml,
                    OPCUA_SUBSCRIPTION_PROTOCOL_TAG_NAME.c_str());
    if (opcua_subscription_protocol_xml == nullptr) {
        // User didn't configure the OPC UA subscription protocol
        return;
    }

    // RequestedPublishingInterval
    struct RTIXMLUTILSObject* requested_publishing_interval_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_subscription_protocol_xml,
                    "requested_publishing_interval");
    if (requested_publishing_interval_xml != nullptr) {
        double requested_publishing_interval = strtod(
                RTIXMLUTILSObject_getText(requested_publishing_interval_xml),
                nullptr);
        subscription_properties.requested_publishing_interval(
                requested_publishing_interval);
    }

    // RequestedLifetimeCount
    struct RTIXMLUTILSObject* requested_lifetime_count_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_subscription_protocol_xml,
                    "requested_lifetime_count");
    if (requested_lifetime_count_xml != nullptr) {
        uint32_t requested_lifetime_count;
        check_type_conversion(
                static_cast<bool>(REDAString_strToUnsignedLong(
                        RTIXMLUTILSObject_getText(requested_lifetime_count_xml),
                        &requested_lifetime_count)),
                requested_lifetime_count_xml);
        subscription_properties.requested_lifetime_count(
                requested_lifetime_count);
    }

    // RequestedMaxAliveCount
    struct RTIXMLUTILSObject* requested_max_keep_alive_count_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_subscription_protocol_xml,
                    "requested_max_keep_alive_count");
    if (requested_max_keep_alive_count_xml != nullptr) {
        uint32_t requested_max_keep_alive_count;
        check_type_conversion(
                static_cast<bool>(REDAString_strToUnsignedLong(
                        RTIXMLUTILSObject_getText(
                                requested_max_keep_alive_count_xml),
                        &requested_max_keep_alive_count)),
                requested_max_keep_alive_count_xml);
        subscription_properties.requested_max_keep_alive_count(
                requested_max_keep_alive_count);
    }

    // MaxNotificationsPerPublish
    struct RTIXMLUTILSObject* max_notifications_per_publish_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_subscription_protocol_xml,
                    "max_notifications_per_publish");
    if (max_notifications_per_publish_xml != nullptr) {
        uint32_t max_notifications_per_publish;
        check_type_conversion(
                static_cast<bool>(REDAString_strToUnsignedLong(
                        RTIXMLUTILSObject_getText(
                                max_notifications_per_publish_xml),
                        &max_notifications_per_publish)),
                max_notifications_per_publish_xml);
        subscription_properties.max_notifications_per_publish(
                max_notifications_per_publish);
    }

    // PublishingEnabled
    struct RTIXMLUTILSObject* publishing_enabled_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_subscription_protocol_xml,
                    "publishing_enabled");
    if (publishing_enabled_xml != nullptr) {
        RTIBool publishing_enabled;
        check_type_conversion(
                static_cast<bool>(REDAString_strToBoolean(
                        RTIXMLUTILSObject_getText(publishing_enabled_xml),
                        &publishing_enabled)),
                publishing_enabled_xml);
        subscription_properties.publishing_enabled(publishing_enabled);
    }

    // Priority
    struct RTIXMLUTILSObject* priority_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_subscription_protocol_xml,
                    "priority");
    if (priority_xml != nullptr) {
        uint16_t priority;
        check_type_conversion(
                static_cast<bool>(REDAString_strToUnsignedShort(
                        RTIXMLUTILSObject_getText(priority_xml),
                        &priority)),
                priority_xml);
        subscription_properties.priority(static_cast<uint8_t>(priority));
    }
}

void XmlOpcUaEndpoint::get_subscription_node_attribute_property(
        std::vector<opcua::sdk::client::MonitoredNodeAttribute>& properties,
        RTIXMLUTILSObject* xml_root,
        const std::string& opcua_endpoint_fqn)
{
    RTIXMLUTILSObject* opcua_endpoint_xml =
            RTIXMLUTILSObject_lookUp(xml_root, opcua_endpoint_fqn.c_str());
    xml_object_check_lookup(opcua_endpoint_xml, xml_root);

    RTIXMLUTILSObject* opcua_node_list_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    opcua_endpoint_xml,
                    OPCUA_MONITORED_ITEMS_TAG_NAME.c_str());
    xml_object_check_lookup(opcua_node_list_xml, opcua_endpoint_xml);

    // Create monitored item properties vector
    RTIXMLUTILSObject* node_xml =
            RTIXMLUTILSObject_getFirstChild(opcua_node_list_xml);

    while (node_xml != nullptr) {
        opcua::sdk::client::MonitoredNodeAttribute node_attribute;

        // Common properties
        get_node_attribute_property(node_attribute, node_xml);

        // Monitoring properties
        opcua::sdk::client::MonitoringProperty monitoring_property;

        // sampling_interval
        struct RTIXMLUTILSObject* sampling_interval_xml =
                RTIXMLUTILSObject_getFirstChildWithTag(
                        node_xml,
                        "sampling_interval");
        if (sampling_interval_xml != nullptr) {
            // TODO: Add support for double sample intervals
            RTI_INT32 sampling_interval;
            check_type_conversion(
                    static_cast<bool>(REDAString_strToLong(
                            RTIXMLUTILSObject_getText(sampling_interval_xml),
                            &sampling_interval)),
                    sampling_interval_xml);
            double interval = static_cast<double>(sampling_interval);
            monitoring_property.sampling_interval(interval);
        }

        // queue size
        struct RTIXMLUTILSObject* queue_size_xml =
                RTIXMLUTILSObject_getFirstChildWithTag(node_xml, "queue_size");
        if (queue_size_xml != nullptr) {
            // TODO: Add support for double sample intervals
            uint32_t queue_size;
            check_type_conversion(
                    static_cast<bool>(REDAString_strToUnsignedLong(
                            RTIXMLUTILSObject_getText(queue_size_xml),
                            &queue_size)),
                    queue_size_xml);

            monitoring_property.queue_size(queue_size);
        }

        // discard_oldest
        struct RTIXMLUTILSObject* discard_oldest_xml =
                RTIXMLUTILSObject_getFirstChildWithTag(
                        node_xml,
                        "discard_oldest");
        if (discard_oldest_xml != nullptr) {
            // TODO: Add support for double sample intervals
            int discard_oldest;
            check_type_conversion(
                    static_cast<bool>(REDAString_strToBoolean(
                            RTIXMLUTILSObject_getText(discard_oldest_xml),
                            &discard_oldest)),
                    discard_oldest_xml);

            monitoring_property.discard_oldest(discard_oldest);
        }

        node_attribute.monitoring_properties(monitoring_property);

        // Push back new monitored item and continue parsing
        properties.push_back(node_attribute);
        node_xml = RTIXMLUTILSObject_getNextSibling(node_xml);
    }
}

void XmlOpcUaEndpoint::get_publication_node_attribute_property(
        std::vector<opcua::sdk::client::PublicationNodeAttribute>& properties,
        RTIXMLUTILSObject* xml_root,
        const std::string& opcua_endpoint_fqn)
{
    RTIXMLUTILSObject* opcua_endpoint_xml =
            RTIXMLUTILSObject_lookUp(xml_root, opcua_endpoint_fqn.c_str());
    xml_object_check_lookup(opcua_endpoint_xml, xml_root);


    RTIXMLUTILSObject* opcua_node_list_xml = opcua_endpoint_xml;
    // Create monitored item properties vector
    RTIXMLUTILSObject* node_xml =
            RTIXMLUTILSObject_getFirstChild(opcua_node_list_xml);
    while (node_xml != nullptr) {
        opcua::sdk::client::PublicationNodeAttribute node_attribute;

        // Set common properties
        get_node_attribute_property(node_attribute, node_xml);

        // Publication properties
        opcua::sdk::client::PublicationProperty publication_properties;

        // Field name
        const char* field_name = RTIXMLUTILSObject_getAttribute(
                node_xml,
                DDS_TOPIC_FIELD_NAME_ATTRIBUTE.c_str());
        xml_object_check_lookup_attribute(
                field_name,
                DDS_TOPIC_FIELD_NAME_ATTRIBUTE.c_str(),
                node_xml);

        publication_properties.field_name(field_name);

        const char* sample_selector = RTIXMLUTILSObject_getAttribute(
                node_xml,
                "dds_sample_selector_ref");
        if (sample_selector != nullptr) {
            publication_properties.sample_selector_name(sample_selector);
        }

        node_attribute.publication_properties(publication_properties);

        // Push back new monitored item and continue parsing
        properties.push_back(node_attribute);
        node_xml = RTIXMLUTILSObject_getNextSibling(node_xml);
    }
}

void XmlOpcUaEndpoint::get_node_attribute_property(
        opcua::sdk::client::NodeAttribute& node_attribute,
        RTIXMLUTILSObject* node_attribute_xml)
{
    // MonitoredItem name
    const char* name = RTIXMLUTILSObject_getAttribute(
            node_attribute_xml,
            DDS_TOPIC_FIELD_NAME_ATTRIBUTE.c_str());
    xml_object_check_lookup_attribute(
            name,
            DDS_TOPIC_FIELD_NAME_ATTRIBUTE.c_str(),
            node_attribute_xml);

    node_attribute.name(name);

    // MonitoredItem AttributeId
    struct RTIXMLUTILSObject* attribute_id_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    node_attribute_xml,
                    OPCUA_ATTRIBUTE_ID_TAG_NAME.c_str());
    xml_object_check_lookup(attribute_id_xml, node_attribute_xml);
    const char* attribute_id = RTIXMLUTILSObject_getText(attribute_id_xml);
    check_text(attribute_id, attribute_id_xml);

    node_attribute.attribute_id(attribute_id);

    // MonitoredItem NodeId
    RTIXMLUTILSObject* node_id_xml = RTIXMLUTILSObject_getFirstChildWithTag(
            node_attribute_xml,
            OPCUA_NODE_ID_TAG_NAME.c_str());
    xml_object_check_lookup(node_id_xml, node_attribute_xml);

    // MonitoredItem NamespaceIndex
    RTIXMLUTILSObject* namespace_index_xml =
            RTIXMLUTILSObject_getFirstChildWithTag(
                    node_id_xml,
                    OPCUA_NAMESPACE_INDEX_TAG_NAME.c_str());
    xml_object_check_lookup(namespace_index_xml, node_id_xml);

    uint16_t namespace_index;
    check_type_conversion(
            static_cast<bool>(REDAString_strToUnsignedShort(
                    RTIXMLUTILSObject_getText(namespace_index_xml),
                    &namespace_index)),
            namespace_index_xml);

    // MonitoredItem NodeId Identifier
    struct RTIXMLUTILSObject* node_identifier_xml = nullptr;
    if ((node_identifier_xml = RTIXMLUTILSObject_getFirstChildWithTag(
                 node_id_xml,
                 "numeric_identifier"))
        != nullptr) {
        uint32_t numeric_identifier;
        check_type_conversion(
                static_cast<bool>(REDAString_strToUnsignedLong(
                        RTIXMLUTILSObject_getText(node_identifier_xml),
                        &numeric_identifier)),
                node_identifier_xml);
        node_attribute.node_id(
                opcua::sdk::types::NodeId(namespace_index, numeric_identifier));
    } else if (
            (node_identifier_xml = RTIXMLUTILSObject_getFirstChildWithTag(
                     node_id_xml,
                     "string_identifier"))
            != nullptr) {
        const char* str_id = RTIXMLUTILSObject_getText(node_identifier_xml);
        node_attribute.node_id(
                opcua::sdk::types::NodeId(namespace_index, (char*) str_id));
    } else {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_XML_UNSUPPORTED_NODE_IDENTIFIER_s,
                RTIXMLUTILSObject_getFullyQualifiedName(namespace_index_xml));
    }
}

}}}  // namespace rti::ddsopcua::config
