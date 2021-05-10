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

#ifndef RTI_DDSOPCUA_TUTORIALS_OPCUA_NODESET_HPP_
#define RTI_DDSOPCUA_TUTORIALS_OPCUA_NODESET_HPP_

#include <iostream>

#include "OpcUaDemoServer.hpp"

namespace rti { namespace ddsopcua { namespace tutorials {

struct RepeatedCallbackParam {
    UA_NodeId node_id;
    int array_length;
};

class OpcUaNodeSet {
public:
    static bool add_nodeset(OpcUaServer& server)
    {
        const char* locale = "en-US";
        int demo_id = 50000;
        const char* demo_name = "Demo";

        // Demo Folder
        UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;
        object_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(demo_name));
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(demo_name));
        UA_NodeId demo_folder_node_id = UA_NODEID_NUMERIC(1, demo_id);
        UA_Server_addObjectNode(
                server.native_server(),
                demo_folder_node_id,
                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>(demo_name)),
                UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
                object_attr,
                nullptr,
                nullptr);

        // Static Folder
        object_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>("001_Static"));
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>("001_Static"));
        UA_NodeId static_folder_node_id = UA_NODEID_NUMERIC(1, 50002);
        UA_Server_addObjectNode(
                server.native_server(),
                static_folder_node_id,
                demo_folder_node_id,
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>("001_Static")),
                UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
                object_attr,
                nullptr,
                nullptr);
        add_scalar_nodes(
                server,
                static_folder_node_id,
                UA_NODEID_NUMERIC(1, 51000));
        add_array_nodes(
                server,
                static_folder_node_id,
                UA_NODEID_NUMERIC(1, 51100));

        // Dynamic Folder
        object_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>("002_Dynamic"));
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>("002_Dynamic"));
        UA_NodeId dynamic_folder_node_id = UA_NODEID_NUMERIC(1, 50003);
        UA_Server_addObjectNode(
                server.native_server(),
                dynamic_folder_node_id,
                demo_folder_node_id,
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>("002_Dynamic")),
                UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
                object_attr,
                nullptr,
                nullptr);

        add_scalar_nodes(
                server,
                dynamic_folder_node_id,
                UA_NODEID_NUMERIC(1, 52000));
        add_array_nodes(
                server,
                dynamic_folder_node_id,
                UA_NODEID_NUMERIC(1, 52100));

        // DDS Folder
        int dds_id = 60000;
        const char* dds_name = "DDS";
        UA_ObjectAttributes dds_object_attr = UA_ObjectAttributes_default;
        dds_object_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(dds_name));
        dds_object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(dds_name));
        UA_NodeId dds_folder_node_id = UA_NODEID_NUMERIC(1, dds_id);
        UA_Server_addObjectNode(
                server.native_server(),
                dds_folder_node_id,
                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>(dds_name)),
                UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
                dds_object_attr,
                nullptr,
                nullptr);

        object_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>("Domain_0"));
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>("Domain_0"));
        UA_NodeId domain_folder_node_id = UA_NODEID_NUMERIC(1, 60001);
        UA_Server_addObjectNode(
                server.native_server(),
                domain_folder_node_id,
                dds_folder_node_id,
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>("Domain_0")),
                UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
                object_attr,
                nullptr,
                nullptr);

        // Add square topic
        UA_NodeId topic_node_id = UA_NODEID_NUMERIC(
                1,
                domain_folder_node_id.identifier.numeric + 1);
        add_topic_node(
                server,
                domain_folder_node_id,
                topic_node_id,
                "Square",
                "ShapeType");

        // Add instances of square topic
        UA_NodeId topic_instance_id =
                UA_NODEID_NUMERIC(1, topic_node_id.identifier.numeric + 100);
        add_topic_instance(server, topic_node_id, topic_instance_id, "BLUE");
        add_shape_topic_instance_members(server, topic_instance_id);

        topic_instance_id = UA_NODEID_NUMERIC(
                1,
                topic_instance_id.identifier.numeric + 100);
        add_topic_instance(server, topic_node_id, topic_instance_id, "RED");
        add_shape_topic_instance_members(server, topic_instance_id);

        topic_instance_id = UA_NODEID_NUMERIC(
                1,
                topic_instance_id.identifier.numeric + 100);
        add_topic_instance(server, topic_node_id, topic_instance_id, "*");
        add_shape_topic_instance_members(server, topic_instance_id);

        // Add MyTopic topic
        topic_node_id =
                UA_NODEID_NUMERIC(1, topic_node_id.identifier.numeric + 1000);
        add_topic_node(
                server,
                domain_folder_node_id,
                topic_node_id,
                "MyTopic",
                "MyType");

        // Add MyTopic Instance
        topic_instance_id =
                UA_NODEID_NUMERIC(1, topic_node_id.identifier.numeric + 100);
        add_topic_instance(
                server,
                topic_node_id,
                topic_instance_id,
                "Instance");
        add_my_topic_instance_members(server, topic_instance_id);

        return true;
    }

    static void add_scalar_nodes(
            OpcUaServer& server,
            const UA_NodeId& parent_node,
            const UA_NodeId& scalar_folder_node_id)
    {
        const char* locale = "en-US";
        const char* scalar_name = "Scalar";

        // Create a Folder Object called "Scalar"
        UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;
        object_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(scalar_name));
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(scalar_name));
        UA_Server_addObjectNode(
                server.native_server(),
                scalar_folder_node_id,
                parent_node,
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>(scalar_name)),
                UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
                object_attr,
                nullptr,
                nullptr);

        const UA_NodeId base_data_variable = {
            0,
            UA_NODEIDTYPE_NUMERIC,
            { UA_NS0ID_BASEDATAVARIABLETYPE }
        };

        // Create Scalar Variable Nodes under the "Scalar" folder
        UA_UInt32 id = scalar_folder_node_id.identifier.numeric + 1;
        for (UA_UInt32 type = 0; type < UA_TYPES_DIAGNOSTICINFO; type++) {
            // Skip unsupported types
            if (type == UA_TYPES_VARIANT || type == UA_TYPES_DIAGNOSTICINFO
                || type == UA_TYPES_EXTENSIONOBJECT
                || type == UA_TYPES_DATAVALUE) {
                continue;
            }
            UA_VariableAttributes attr = UA_VariableAttributes_default;
            attr.displayName = UA_LOCALIZEDTEXT_ALLOC(
                    const_cast<char*>(locale),
                    UA_TYPES[type].typeName);
            UA_QualifiedName qualified_name =
                    UA_QUALIFIEDNAME_ALLOC(1, UA_TYPES[type].typeName);
            attr.accessLevel =
                    UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            attr.valueRank = UA_VALUERANK_SCALAR;
            void* value = UA_new(&UA_TYPES[type]);
            UA_Variant_setScalar(&attr.value, value, &UA_TYPES[type]);
            UA_Server_addVariableNode(
                    server.native_server(),
                    UA_NODEID_NUMERIC(1, id++),
                    scalar_folder_node_id,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                    qualified_name,
                    base_data_variable,
                    attr,
                    nullptr,
                    nullptr);
            UA_Variant_deleteMembers(&attr.value);
            UA_LocalizedText_deleteMembers(&attr.displayName);
            UA_QualifiedName_deleteMembers(&qualified_name);
        }
    }

    static void add_array_nodes(
            OpcUaServer& server,
            const UA_NodeId& parent_node,
            const UA_NodeId& array_folder_node_id)
    {
        const char* locale = "en-US";
        const char* array_name = "Arrays";

        // Create a Folder Object called "Arrays"
        UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;
        object_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(array_name));
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale),
                const_cast<char*>(array_name));

        UA_Server_addObjectNode(
                server.native_server(),
                array_folder_node_id,
                parent_node,
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>(array_name)),
                UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
                object_attr,
                nullptr,
                nullptr);

        const UA_NodeId base_data_variable = {
            0,
            UA_NODEIDTYPE_NUMERIC,
            { UA_NS0ID_BASEDATAVARIABLETYPE }
        };

        // Create Array Variable Nodes under the "Arrays" folder
        UA_UInt32 id = array_folder_node_id.identifier.numeric + 1;
        for (UA_UInt32 type = 0; type < UA_TYPES_DIAGNOSTICINFO; type++) {
            // Skip unsupported types
            if (type == UA_TYPES_VARIANT || type == UA_TYPES_DIAGNOSTICINFO
                || type == UA_TYPES_EXTENSIONOBJECT
                || type == UA_TYPES_DATAVALUE) {
                continue;
            }

            UA_VariableAttributes attr = UA_VariableAttributes_default;
            attr.displayName = UA_LOCALIZEDTEXT_ALLOC(
                    const_cast<char*>(locale),
                    UA_TYPES[type].typeName);
            UA_QualifiedName qualified_name =
                    UA_QUALIFIEDNAME_ALLOC(1, UA_TYPES[type].typeName);
            /* add an array node for every built-in type */
            UA_UInt32 arrayDims = 0;
            attr.accessLevel =
                    UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            attr.valueRank = UA_VALUERANK_ONE_DIMENSION;
            attr.arrayDimensions = &arrayDims;
            attr.arrayDimensionsSize = 1;
            UA_Variant_setArray(
                    &attr.value,
                    UA_Array_new(10, &UA_TYPES[type]),
                    10,
                    &UA_TYPES[type]);
            UA_StatusCode retcode = UA_Server_addVariableNode(
                    server.native_server(),
                    UA_NODEID_NUMERIC(1, id++),
                    UA_NODEID_NUMERIC(
                            1,
                            array_folder_node_id.identifier.numeric),
                    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                    qualified_name,
                    base_data_variable,
                    attr,
                    nullptr,
                    nullptr);
            if (retcode != UA_STATUSCODE_GOOD) {
                std::cerr << "Error creating " << qualified_name.name.data
                          << std::endl;
            }
            UA_Variant_deleteMembers(&attr.value);
            UA_LocalizedText_deleteMembers(&attr.displayName);
            UA_QualifiedName_deleteMembers(&qualified_name);
        }
    }

    static void add_topic_node(
            OpcUaServer& server,
            const UA_NodeId& parent_node,
            const UA_NodeId& topic_folder_id,
            const std::string& topic_name,
            const std::string& registered_name)
    {
        UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>("en-US"),
                const_cast<char*>(topic_name.c_str()));
        UA_StatusCode retcode = UA_Server_addObjectNode(
                server.native_server(),
                topic_folder_id,
                parent_node,
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>(topic_name.c_str())),
                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                object_attr,
                nullptr,
                nullptr);
        if (retcode != UA_STATUSCODE_GOOD) {
            std::cerr << "Error creating node " << topic_name << std::endl;
        }
        UA_VariableAttributes variable_attr = UA_VariableAttributes_default;
        variable_attr.description = UA_LOCALIZEDTEXT(
                const_cast<char*>("en-US"),
                const_cast<char*>("RegisteredTypeName"));
        variable_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>("en-US"),
                const_cast<char*>("RegisteredTypeName"));

        // Add RegisteredTypeName
        UA_String registered_type_name =
                UA_STRING(const_cast<char*>(registered_name.c_str()));
        uint32_t registered_type_name_id =
                topic_folder_id.identifier.numeric + 1;
        UA_Variant_setScalar(
                &variable_attr.value,
                &registered_type_name,
                &UA_TYPES[UA_TYPES_STRING]);
        retcode = UA_Server_addVariableNode(
                server.native_server(),
                UA_NODEID_NUMERIC(1, registered_type_name_id),
                topic_folder_id,
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                UA_QUALIFIEDNAME(1, const_cast<char*>("RegisteredTypeName")),
                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                variable_attr,
                nullptr,
                nullptr);
        if (retcode != UA_STATUSCODE_GOOD) {
            std::cerr << "Error creating node RegisteredTypeName" << std::endl;
        }
    }

    static void add_topic_instance(
            OpcUaServer& server,
            const UA_NodeId& parent_node,
            const UA_NodeId& instance_folder_id,
            const std::string& instance_name)
    {
        UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;
        object_attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>("en-US"),
                const_cast<char*>(instance_name.c_str()));
        UA_StatusCode retcode = UA_Server_addObjectNode(
                server.native_server(),
                instance_folder_id,
                parent_node,
                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                UA_QUALIFIEDNAME(1, const_cast<char*>(instance_name.c_str())),
                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                object_attr,
                nullptr,
                nullptr);
        if (retcode != UA_STATUSCODE_GOOD) {
            std::cerr << "Error creating Node " << instance_name << std::endl;
        }
    }

    static void add_scalar_instance_member(
            OpcUaServer& server,
            const UA_NodeId& parent_node,
            const UA_NodeId& node_id,
            const std::string& member_name,
            const uint32_t member_type)
    {
        std::string locale = "en-US";
        UA_VariableAttributes attr = UA_VariableAttributes_default;
        attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        attr.valueRank = UA_VALUERANK_SCALAR;
        attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale.c_str()),
                const_cast<char*>(member_name.c_str()));
        UA_Variant_setScalar(&attr.value, nullptr, &UA_TYPES[member_type]);
        UA_Server_addVariableNode(
                server.native_server(),
                node_id,
                parent_node,
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                UA_QUALIFIEDNAME(1, const_cast<char*>(member_name.c_str())),
                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                attr,
                nullptr,
                nullptr);
    }

    static void add_array_instance_member(
            OpcUaServer& server,
            const UA_NodeId& parent_node,
            const UA_NodeId& node_id,
            const std::string& member_name,
            const uint32_t member_type)
    {
        std::string locale = "en-US";
        uint32_t array_dimensions = 0;
        UA_VariableAttributes attr = UA_VariableAttributes_default;
        attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        attr.valueRank = UA_VALUERANK_ONE_DIMENSION;
        attr.arrayDimensionsSize = 1;
        attr.arrayDimensions = &array_dimensions;
        attr.displayName = UA_LOCALIZEDTEXT(
                const_cast<char*>(locale.c_str()),
                const_cast<char*>(member_name.c_str()));
        UA_Variant_setArray(&attr.value, nullptr, 0, &UA_TYPES[member_type]);
        UA_Server_addVariableNode(
                server.native_server(),
                node_id,
                parent_node,
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                UA_QUALIFIEDNAME(1, const_cast<char*>(member_name.c_str())),
                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                attr,
                nullptr,
                nullptr);
    }

    static void add_shape_topic_instance_members(
            OpcUaServer& server,
            const UA_NodeId& parent_node)
    {
        uint32_t numeric_node_id = parent_node.identifier.numeric;
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "color",
                UA_TYPES_STRING);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "x",
                UA_TYPES_INT32);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "y",
                UA_TYPES_INT32);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "shapesize",
                UA_TYPES_INT32);
    }

    static void add_my_topic_instance_members(
            OpcUaServer& server,
            const UA_NodeId& parent_node)
    {
        uint32_t numeric_node_id = parent_node.identifier.numeric;
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_boolean",
                UA_TYPES_BOOLEAN);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_byte",
                UA_TYPES_BYTE);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int16",
                UA_TYPES_INT16);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint16",
                UA_TYPES_UINT16);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int32",
                UA_TYPES_INT32);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint32",
                UA_TYPES_UINT32);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int64",
                UA_TYPES_INT64);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint64",
                UA_TYPES_UINT64);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_float",
                UA_TYPES_FLOAT);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_double",
                UA_TYPES_DOUBLE);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_char",
                UA_TYPES_STRING);
        add_scalar_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_string",
                UA_TYPES_STRING);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_boolean_array",
                UA_TYPES_BOOLEAN);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_byte_array",
                UA_TYPES_BYTE);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int16_array",
                UA_TYPES_INT16);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint16_array",
                UA_TYPES_UINT16);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int32_array",
                UA_TYPES_INT32);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint32_array",
                UA_TYPES_UINT32);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int64_array",
                UA_TYPES_INT64);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint64_array",
                UA_TYPES_UINT64);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_float_array",
                UA_TYPES_FLOAT);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_double_array",
                UA_TYPES_DOUBLE);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_char_array",
                UA_TYPES_STRING);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_string_array",
                UA_TYPES_STRING);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_boolean_sequence",
                UA_TYPES_BOOLEAN);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_byte_sequence",
                UA_TYPES_BYTE);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int16_sequence",
                UA_TYPES_INT16);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint16_sequence",
                UA_TYPES_UINT16);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int32_sequence",
                UA_TYPES_INT32);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint32_sequence",
                UA_TYPES_UINT32);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_int64_sequence",
                UA_TYPES_INT64);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_uint64_sequence",
                UA_TYPES_UINT64);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_float_sequence",
                UA_TYPES_FLOAT);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_double_sequence",
                UA_TYPES_DOUBLE);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_char_sequence",
                UA_TYPES_STRING);
        add_array_instance_member(
                server,
                parent_node,
                UA_NODEID_NUMERIC(1, ++numeric_node_id),
                "my_string_sequence",
                UA_TYPES_STRING);
    }

    static void update_scalar_node_values(UA_Server* server, void* param)
    {
        UA_NodeId dynamic_scalar_nodeid = *(UA_NodeId*) param;
        UA_UInt32 id = dynamic_scalar_nodeid.identifier.numeric + 1;
        for (UA_UInt32 type = 0; type < UA_TYPES_DIAGNOSTICINFO; type++) {
            // Skip unsupported types
            if (type == UA_TYPES_VARIANT || type == UA_TYPES_DIAGNOSTICINFO
                || type == UA_TYPES_EXTENSIONOBJECT
                || type == UA_TYPES_DATAVALUE) {
                continue;
            }

            update_scalar_node_value(server, UA_NODEID_NUMERIC(1, id++), type);
        }
    }

    static void update_array_node_values(UA_Server* server, void* param)
    {
        RepeatedCallbackParam dynamic_array_nodeid =
                *(RepeatedCallbackParam*) param;
        UA_UInt32 id = dynamic_array_nodeid.node_id.identifier.numeric + 1;
        for (UA_UInt32 type = 0; type < UA_TYPES_DIAGNOSTICINFO; type++) {
            // Skip unsupported types
            if (type == UA_TYPES_VARIANT || type == UA_TYPES_DIAGNOSTICINFO
                || type == UA_TYPES_EXTENSIONOBJECT
                || type == UA_TYPES_DATAVALUE) {
                continue;
            }

            update_array_node_value(
                    server,
                    UA_NODEID_NUMERIC(1, id++),
                    type,
                    dynamic_array_nodeid.array_length);
        }
    }


    static void update_scalar_node_value(
            UA_Server* server,
            const UA_NodeId& node_id,
            const size_t type_id)
    {
        static UA_Int64 i = 1;
        UA_Variant variant;
        switch (type_id) {
        case UA_TYPES_BOOLEAN: {
            bool value = ((i - 1) % 2);
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_BOOLEAN]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_SBYTE: {
            UA_SByte value =
                    ((i % 2)) ? (i % UA_SBYTE_MAX) : -(i % UA_SBYTE_MAX);
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_SBYTE]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_BYTE: {
            UA_Byte value = (i % UA_BYTE_MAX);
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_BYTE]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_INT16: {
            UA_Int16 value =
                    ((i % 2) == 1) ? (i % UA_INT16_MAX) : -((i % UA_INT16_MAX));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_INT16]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_UINT16: {
            UA_UInt16 value = (i % UA_UINT16_MAX);
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_UINT16]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_INT32: {
            UA_Int32 value =
                    ((i % 2) == 1) ? (i % UA_INT32_MAX) : -((i % UA_INT32_MAX));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_INT32]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_UINT32: {
            UA_UInt32 value = (i % UA_UINT32_MAX);
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_UINT32]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_INT64: {
            UA_Int64 value =
                    ((i % 2) == 1) ? (i % UA_INT64_MAX) : -((i % UA_INT64_MAX));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_INT64]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_UINT64: {
            UA_UInt64 value = (i % UA_INT64_MAX);
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_UINT64]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_FLOAT: {
            float point5 = 0.5;
            UA_Float value = (i % UA_INT32_MAX) * point5;
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_FLOAT]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_DOUBLE: {
            UA_Double value = (i % UA_INT32_MAX) * 0.5;
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_DOUBLE]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_STRING: {
            UA_String value = (i % 2) ? UA_STRING(const_cast<char*>("Odd"))
                                      : UA_STRING(const_cast<char*>("Even"));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_STRING]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_DATETIME: {
            UA_DateTime value =
                    ((i % 2) == 1) ? (i % UA_INT64_MAX) : -((i % UA_INT64_MAX));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_DATETIME]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_GUID: {
            UA_Guid value = { static_cast<UA_UInt32>(i % UA_UINT32_MAX),
                              static_cast<UA_UInt16>(i % UA_UINT16_MAX),
                              static_cast<UA_UInt16>(i % UA_UINT16_MAX),
                              { static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                                static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                                static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                                static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                                static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                                static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                                static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                                static_cast<UA_Byte>(i % UA_SBYTE_MAX) } };
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_GUID]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_BYTESTRING: {
            UA_ByteString value = ((i % 2) == 1)
                    ? UA_BYTESTRING(const_cast<char*>("Odd"))
                    : UA_BYTESTRING(const_cast<char*>("Even"));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_BYTESTRING]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_XMLELEMENT: {
            UA_XmlElement value = ((i % 2) == 1)
                    ? UA_STRING(const_cast<char*>("<Odd></Odd>"))
                    : UA_STRING(const_cast<char*>("<Even></Even>"));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_XMLELEMENT]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_NODEID: {
            UA_NodeId value;
            UA_UInt16 ns = 42;
            if ((i % 4) == 1) {
                value = UA_NODEID_NUMERIC(
                        ns,
                        static_cast<UA_UInt32>(i % UA_UINT32_MAX));
            } else if ((i % 4) == 2) {
                value = UA_NODEID_STRING(ns, const_cast<char*>("Odd"));
            } else if ((i % 4) == 3) {
                value = UA_NODEID_BYTESTRING(ns, const_cast<char*>("Even"));
            } else {
                UA_Guid guid_value = {
                    static_cast<UA_UInt32>(i % UA_UINT32_MAX),
                    static_cast<UA_UInt16>(i % UA_UINT16_MAX),
                    static_cast<UA_UInt16>(i % UA_UINT16_MAX),
                    { static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX) }
                };
                value = UA_NODEID_GUID(ns, guid_value);
            }
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_NODEID]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_EXPANDEDNODEID: {
            UA_ExpandedNodeId value;
            UA_UInt16 ns = 42;
            if ((i % 4) == 1) {
                value = UA_EXPANDEDNODEID_NUMERIC(
                        ns,
                        static_cast<UA_UInt32>(i % UA_UINT32_MAX));
            } else if ((i % 4) == 2) {
                value = UA_EXPANDEDNODEID_STRING(ns, const_cast<char*>("Odd"));
            } else if ((i % 4) == 3) {
                value = UA_EXPANDEDNODEID_BYTESTRING(
                        ns,
                        const_cast<char*>("Even"));
            } else {
                UA_Guid guid_value = {
                    static_cast<UA_UInt32>(i % UA_UINT32_MAX),
                    static_cast<UA_UInt16>(i % UA_UINT16_MAX),
                    static_cast<UA_UInt16>(i % UA_UINT16_MAX),
                    { static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX),
                      static_cast<UA_Byte>(i % UA_SBYTE_MAX) }
                };
                value = UA_EXPANDEDNODEID_STRING_GUID(ns, guid_value);
            }
            value.namespaceUri =
                    UA_STRING(const_cast<char*>("http://www.rti.com"));

            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_EXPANDEDNODEID]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_STATUSCODE: {
            UA_StatusCode value = static_cast<UA_UInt32>(i % UA_UINT32_MAX);
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_STATUSCODE]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_QUALIFIEDNAME: {
            UA_UInt16 ns = 42;
            UA_QualifiedName value = (i % 2)
                    ? UA_QUALIFIEDNAME(ns, const_cast<char*>("Odd"))
                    : UA_QUALIFIEDNAME(ns, const_cast<char*>("Even"));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        case UA_TYPES_LOCALIZEDTEXT: {
            UA_LocalizedText value = (i % 2) ? UA_LOCALIZEDTEXT(
                                             const_cast<char*>("en-US"),
                                             const_cast<char*>("Odd"))
                                             : UA_LOCALIZEDTEXT(
                                                     const_cast<char*>("en-US"),
                                                     const_cast<char*>("Even"));
            UA_Variant_setScalar(
                    &variant,
                    (void*) &value,
                    &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
            UA_Server_writeValue(server, node_id, variant);
        } break;
        }

        i++;
        if (i == 0) {
            // If it rolls -- don't let it get to zero
            i++;
        }
    }

    static void update_array_node_value(
            UA_Server* server,
            const UA_NodeId& node_id,
            const size_t type_id,
            const int array_length)
    {
        static UA_Int64 seed = 1;
        UA_Variant variant;
        switch (type_id) {
        case UA_TYPES_BOOLEAN: {
            bool* value = new bool[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed - 1) % 2);
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_BOOLEAN]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_SBYTE: {
            UA_SByte* value = new UA_SByte[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed % 2)) ? (seed % UA_SBYTE_MAX)
                                        : -(seed % UA_SBYTE_MAX);
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_SBYTE]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_BYTE: {
            UA_Byte* value = new UA_Byte[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % UA_BYTE_MAX);
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_BYTE]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_INT16: {
            UA_Int16* value = new UA_Int16[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed % 2) == 1) ? (seed % UA_INT16_MAX)
                                             : -((seed % UA_INT16_MAX));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_INT16]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_UINT16: {
            UA_UInt16* value = new UA_UInt16[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % UA_UINT16_MAX);
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_UINT16]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_INT32: {
            UA_Int32* value = new UA_Int32[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed % 2) == 1) ? (seed % UA_INT32_MAX)
                                             : -((seed % UA_INT32_MAX));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_INT32]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_UINT32: {
            UA_UInt32* value = new UA_UInt32[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % UA_UINT32_MAX);
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_UINT32]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_INT64: {
            UA_Int64* value = new UA_Int64[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed % 2) == 1) ? (seed % UA_INT64_MAX)
                                             : -((seed % UA_INT64_MAX));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_INT64]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_UINT64: {
            UA_UInt64* value = new UA_UInt64[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % UA_INT64_MAX);
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_UINT64]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_FLOAT: {
            UA_Float* value = new UA_Float[array_length];
            for (int i = 0; i < array_length; i++) {
                float point5 = 0.5;
                value[i] = (seed % UA_INT32_MAX) * point5;
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_FLOAT]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_DOUBLE: {
            UA_Double* value = new UA_Double[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % UA_INT32_MAX) * 0.5;
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_DOUBLE]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_STRING: {
            UA_String* value = new UA_String[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % 2) ? UA_STRING(const_cast<char*>("Odd"))
                                      : UA_STRING(const_cast<char*>("Even"));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_STRING]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_DATETIME: {
            UA_DateTime* value = new UA_DateTime[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed % 2) == 1) ? (seed % UA_INT64_MAX)
                                             : -((seed % UA_INT64_MAX));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_DATETIME]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_GUID: {
            UA_Guid* value = new UA_Guid[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i].data1 = static_cast<UA_UInt32>(seed % UA_UINT32_MAX);
                value[i].data2 = static_cast<UA_UInt16>(seed % UA_UINT16_MAX);
                value[i].data3 = static_cast<UA_UInt16>(seed % UA_UINT16_MAX);
                for (int j = 0; j < 8; j++) {
                    value[i].data4[j] =
                            static_cast<UA_Byte>(seed % UA_BYTE_MAX);
                }
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_GUID]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_BYTESTRING: {
            UA_ByteString* value = new UA_ByteString[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed % 2) == 1)
                        ? UA_BYTESTRING(const_cast<char*>("Odd"))
                        : UA_BYTESTRING(const_cast<char*>("Even"));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_BYTESTRING]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_XMLELEMENT: {
            UA_XmlElement* value = new UA_XmlElement[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = ((seed % 2) == 1)
                        ? UA_STRING(const_cast<char*>("<Odd></Odd>"))
                        : UA_STRING(const_cast<char*>("<Even></Even>"));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_XMLELEMENT]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_NODEID: {
            UA_NodeId* value = new UA_NodeId[array_length];
            UA_UInt16 ns = 42;
            for (int i = 0; i < array_length; i++) {
                if ((seed % 4) == 1) {
                    value[i] = UA_NODEID_NUMERIC(
                            ns,
                            static_cast<UA_UInt32>(seed % UA_UINT32_MAX));
                } else if ((seed % 4) == 2) {
                    value[i] = UA_NODEID_STRING(ns, const_cast<char*>("Odd"));
                } else if ((seed % 4) == 3) {
                    value[i] =
                            UA_NODEID_BYTESTRING(ns, const_cast<char*>("Even"));
                } else {
                    UA_Guid guid_value = {
                        static_cast<UA_UInt32>(seed % UA_UINT32_MAX),
                        static_cast<UA_UInt16>(seed % UA_UINT16_MAX),
                        static_cast<UA_UInt16>(seed % UA_UINT16_MAX),
                        { static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX) }
                    };
                    value[i] = UA_NODEID_GUID(ns, guid_value);
                }
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_NODEID]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_EXPANDEDNODEID: {
            UA_ExpandedNodeId* value = new UA_ExpandedNodeId[array_length];
            UA_UInt16 ns = 42;
            for (int i = 0; i < array_length; i++) {
                if ((seed % 4) == 1) {
                    value[i] = UA_EXPANDEDNODEID_NUMERIC(
                            ns,
                            static_cast<UA_UInt32>(seed % UA_UINT32_MAX));
                } else if ((seed % 4) == 2) {
                    value[i] = UA_EXPANDEDNODEID_STRING(
                            ns,
                            const_cast<char*>("Odd"));
                } else if ((seed % 4) == 3) {
                    value[i] = UA_EXPANDEDNODEID_BYTESTRING(
                            ns,
                            const_cast<char*>("Even"));
                } else {
                    UA_Guid guid_value = {
                        static_cast<UA_UInt32>(seed % UA_UINT32_MAX),
                        static_cast<UA_UInt16>(seed % UA_UINT16_MAX),
                        static_cast<UA_UInt16>(seed % UA_UINT16_MAX),
                        { static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX),
                          static_cast<UA_Byte>(seed % UA_SBYTE_MAX) }
                    };
                    value[i] = UA_EXPANDEDNODEID_STRING_GUID(ns, guid_value);
                }
                value[i].namespaceUri =
                        UA_STRING(const_cast<char*>("http://www.rti.com"));
            }

            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_EXPANDEDNODEID]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_STATUSCODE: {
            UA_StatusCode* value = new UA_StatusCode[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = static_cast<UA_UInt32>(seed % UA_UINT32_MAX);
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_STATUSCODE]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_QUALIFIEDNAME: {
            UA_UInt16 ns = 42;
            UA_QualifiedName* value = new UA_QualifiedName[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % 2)
                        ? UA_QUALIFIEDNAME(ns, const_cast<char*>("Odd"))
                        : UA_QUALIFIEDNAME(ns, const_cast<char*>("Even"));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        case UA_TYPES_LOCALIZEDTEXT: {
            UA_LocalizedText* value = new UA_LocalizedText[array_length];
            for (int i = 0; i < array_length; i++) {
                value[i] = (seed % 2) ? UA_LOCALIZEDTEXT(
                                   const_cast<char*>("en-US"),
                                   const_cast<char*>("Odd"))
                                      : UA_LOCALIZEDTEXT(
                                              const_cast<char*>("en-US"),
                                              const_cast<char*>("Even"));
            }
            UA_Variant_setArray(
                    &variant,
                    (void*) value,
                    array_length,
                    &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
            UA_Server_writeValue(server, node_id, variant);
            delete[] value;
        } break;
        }

        seed++;
        if (seed == 0) {
            // If it rolls -- don't let it get to zero
            seed++;
        }
    }
};

}}}  // namespace rti::ddsopcua::tutorials

#endif  // RTI_DDSOPCUA_TUTORIALS_OPCUA_NODESET_HPP_