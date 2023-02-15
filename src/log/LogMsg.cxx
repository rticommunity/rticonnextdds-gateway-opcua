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

#include "log/LogMsg.hpp"

const RTILogMessage DDSOPCUA_LOG_OPCUA_ADD_MONITORED_ITEM_FAILED_ss = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 0,
    "Error adding monitored item '%s' to OPC UA subscription. The operation "
    "returned '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_OPCUA_DELETE_SUBSCRIPTION_FAILED_u = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 1,
    "Error deleting OPC UA subscription '%u'\n"
};

const RTILogMessage DDSOPCUA_LOG_OPCUA_CREATE_SUBSCRIPTION_FAILED_u = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 2,
    "Error creating OPC UA subscription '%u'\n"
};

const RTILogMessage DDSOPCUA_LOG_OPCUA_CONNECT_SERVER_FAILED_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 3,
    "Error connecting to OPC UA server '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_XML_LOOKUP_ATTRIBUTE_ss = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 4,
    "Cannot lookup attribute '%s' from object '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_XML_CONVERT_OBJECT_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 5,
    "Cannot convert content from XML object '%s' to the appropriate type\n"
};

const RTILogMessage DDSOPCUA_LOG_XML_UNSUPPORTED_NODE_IDENTIFIER_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 6,
    "Unsupported Node Identifier in '%s'. Only Numeric and String "
    "Identifiers are supported\n"
};

const RTILogMessage DDSOPCUA_LOG_SEMAPHORE_TAKE_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 7,
    "Cannot take semaphore. Opeartion returned status error '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_THREAD_CREATE_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 8,
    "Cannot create thread '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_OPCUA_RUN_ASYNC_FAILURE_d = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 9,
    "Error running asynchronous client with timeout '%u'\n"
};

const RTILogMessage DDSOPCUA_LOG_XML_READ_ATTRIBUTE_suss = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 10,
    "Cannot read attribute '%s' from node '%u,%s' failed with error '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_XML_WRITE_ATTRIBUTE_suss = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 11,
    "Cannot write attribute '%s' from node '%u,%s' failed with error '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_OPCUA_UNKNOWN_ATTRIBUTE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 12,
    "Cannot process unknown attribute '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_OPCUA_UNKNOWN_ATTRIBUTE_u = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 13,
    "Cannot process unknown attribute '%u'\n"
};
const RTILogMessage DDSOPCUA_LOG_XML_LOOKUP_SELETOR_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 14,
    "!lookup selector '%s' referenced by node attributes in "
    "an opcua_output. All selectors must be previously registered "
    "in the corresponding dds_input\n"
};

const RTILogMessage DDSOPCUA_LOG_OPCUA_DISCONNECT_SERVER_FAILED_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 15,
    "Cannot disconnect client, failed with status code '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_ADD_MONITORED_ITEM_sfud = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 16,
    "Adding monitored item '%s', with sampling_interval %f, "
    "queue_size %u, and discard_oldest %d\n"
};

const RTILogMessage DDSOPCUA_LOG_ADDED_MONITORED_ITEM_ufu = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 17,
    "Added monitored item with ID '%u', with revised_sampling_interval %f "
    "and revised_queue_size %u\n"
};

const RTILogMessage DDSOPCUA_LOG_CREATE_SUBSCRIPTION_uduuud = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 18,
    "Creating subscription with max_notifications_per_publish %u, "
    "requested_publishing_interval %f, requested_max_keep_alive_count %u, "
    "max_notifications_per_publish %u, requested_lifetime_count %u, "
    "and priority %d\n"
};

const RTILogMessage DDSOPCUA_LOG_CREATED_SUBSCRIPTION_ufuu = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 19,
    "Created subscription with ID %u, revised_publishing_interval %f "
    "revised_max_keep_alive_count %u, and revised_lifetime_count %u\n"
};

const RTILogMessage DDSOPCUA_LOG_PROCESS_MONITORED_ITEM_uu = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 20,
    "Processing data available on subscription %u for monitored item %u\n"
};
const RTILogMessage DDSOPCUA_LOG_OPCUA_UNSUPPORTED_ATTRIBUTE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 21,
    "Cannot process unsupported attribute kind '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_CREATE_STREAMREADER_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 22,
    "Error creating stream reader '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_XML_LOOKUP_PARENT_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 23,
    "Error looking up XML Object from parent '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_ANY_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 24,
    "%s\n"
};

const RTILogMessage DDSOPCUA_LOG_ANY_ss = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 25,
    "%s%s\n"
};

const RTILogMessage DDSOPCUA_LOG_CREATION_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 26,
    "Error creating %s\n"
};

const RTILogMessage DDSOPCUA_LOG_ANY_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 27,
    "Error: %s\n"
};

const RTILogMessage DDSOPCUA_LOG_INIT_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 28,
    "Error initializing '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_PARSER_ARGUMENT_MISSING_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 29,
    "Error parsing '%s': argument missing\n"
};

const RTILogMessage DDSOPCUA_LOG_PARSER_FAILURE_ss = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 30,
    "Error parsing '%s': %s\n"
};

const RTILogMessage DDSOPCUA_LOG_PARSER_PARSE_FILE_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 31,
    "Error parsing file '%s': %s\n"
};

const RTILogMessage DDSOPCUA_LOG_PARSER_PARSE_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 32,
    "Error parsing XML file: %s\n"
};

const RTILogMessage DDSOPCUA_LOG_PARSER_LOAD_FILE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 33,
    "Loading XML file: '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_VALIDATOR_VALIDATE_FILE_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 34,
    "Error validating XML file: '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_TRANSFORMER_TRANSFORM_FAILURE_ss = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 35,
    "Error transforming from '%s' to '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_VALIDATOR_VALIDATE_ENTITY_FAILURE_s = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 36,
    "Error valating entity with tag '%s'\n"
};

const RTILogMessage DDSOPCUA_LOG_UNEXPECTED_EXCEPTION = {
    PRODUCT_WAVEWORKS | MODULE_DDSOPCUA | RTI_SUBMODULE_MASK_ALL | 37,
    "Unexpected exception\n"
};
