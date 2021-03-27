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

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include "OpcUaSdkDataTypes.hpp"
#include "log/LogMsg.hpp"


namespace rti { namespace opcua { namespace sdk { namespace types {

// const NodeId BOOLEAN_NODE_ID = UA_NODEID_NUMERIC(0, UA_NS0ID_BOOLEAN);

std::string AttributeId::attribute_id_to_attribute_id_string(
        const uint32_t attribute_id)
{
    std::string attribute_id_str;
    switch (attribute_id) {
    case UA_ATTRIBUTEID_NODEID:
        attribute_id_str = "NODEID";
        break;
    case UA_ATTRIBUTEID_NODECLASS:
        attribute_id_str = "NODECLASS";
        break;
    case UA_ATTRIBUTEID_BROWSENAME:
        attribute_id_str = "BROWSENAME";
        break;
    case UA_ATTRIBUTEID_DISPLAYNAME:
        attribute_id_str = "DISPLAYNAME";
        break;
    case UA_ATTRIBUTEID_DESCRIPTION:
        attribute_id_str = "DESCRIPTION";
        break;
    case UA_ATTRIBUTEID_WRITEMASK:
        attribute_id_str = "WRITEMASK";
        break;
    case UA_ATTRIBUTEID_USERWRITEMASK:
        attribute_id_str = "USERWRITEMASK";
        break;
    case UA_ATTRIBUTEID_ISABSTRACT:
        attribute_id_str = "ISABSTRACT";
        break;
    case UA_ATTRIBUTEID_SYMMETRIC:
        attribute_id_str = "SYMMETRIC";
        break;
    case UA_ATTRIBUTEID_INVERSENAME:
        attribute_id_str = "INVERSENAME";
        break;
    case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
        attribute_id_str = "CONTAINSNOLOOPS";
        break;
    case UA_ATTRIBUTEID_EVENTNOTIFIER:
        attribute_id_str = "EVENTNOTIFIER";
        break;
    case UA_ATTRIBUTEID_VALUE:
        attribute_id_str = "VALUE";
        break;
    case UA_ATTRIBUTEID_DATATYPE:
        attribute_id_str = "DATATYPE";
        break;
    case UA_ATTRIBUTEID_VALUERANK:
        attribute_id_str = "VALUERANK";
        break;
    case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
        attribute_id_str = "ARRAYDIMENSIONS";
        break;
    case UA_ATTRIBUTEID_ACCESSLEVEL:
        attribute_id_str = "ACCESSLEVEL";
        break;
    case UA_ATTRIBUTEID_USERACCESSLEVEL:
        attribute_id_str = "USERACCESSLEVEL";
        break;
    case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
        attribute_id_str = "MINIMUMSAMPLINGINTERVAL";
        break;
    case UA_ATTRIBUTEID_HISTORIZING:
        attribute_id_str = "HISTORIZING";
        break;
    case UA_ATTRIBUTEID_EXECUTABLE:
        attribute_id_str = "EXECUTABLE";
        break;
    case UA_ATTRIBUTEID_USEREXECUTABLE:
        attribute_id_str = "USEREXECUTABLE";
        break;
    case UA_ATTRIBUTEID_DATATYPEDEFINITION:
        attribute_id_str = "DATATYPEDEFINITION";
        break;
    default:
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_OPCUA_UNKNOWN_ATTRIBUTE_u,
                static_cast<uint32_t>(attribute_id));
    }

    return attribute_id_str;
}

UA_AttributeId AttributeId::attribute_id_string_to_attribute_id(
        const std::string& attribute_id_str)
{
    UA_AttributeId attribute_id = UA_ATTRIBUTEID_VALUE;
    if (attribute_id_str == "NODEID") {
        attribute_id = UA_ATTRIBUTEID_NODEID;
    } else if (attribute_id_str == "NODECLASS") {
        attribute_id = UA_ATTRIBUTEID_NODECLASS;
    } else if (attribute_id_str == "BROWSENAME") {
        attribute_id = UA_ATTRIBUTEID_BROWSENAME;
    } else if (attribute_id_str == "DISPLAYNAME") {
        attribute_id = UA_ATTRIBUTEID_DISPLAYNAME;
    } else if (attribute_id_str == "DESCRIPTION") {
        attribute_id = UA_ATTRIBUTEID_DESCRIPTION;
    } else if (attribute_id_str == "WRITEMASK") {
        attribute_id = UA_ATTRIBUTEID_WRITEMASK;
    } else if (attribute_id_str == "USERWRITEMASK") {
        attribute_id = UA_ATTRIBUTEID_USERWRITEMASK;
    } else if (attribute_id_str == "ISABSTRACT") {
        attribute_id = UA_ATTRIBUTEID_ISABSTRACT;
    } else if (attribute_id_str == "SYMMETRIC") {
        attribute_id = UA_ATTRIBUTEID_SYMMETRIC;
    } else if (attribute_id_str == "INVERSENAME") {
        attribute_id = UA_ATTRIBUTEID_INVERSENAME;
    } else if (attribute_id_str == "CONTAINSNOLOOPS") {
        attribute_id = UA_ATTRIBUTEID_CONTAINSNOLOOPS;
    } else if (attribute_id_str == "EVENTNOTIFIER") {
        attribute_id = UA_ATTRIBUTEID_EVENTNOTIFIER;
    } else if (attribute_id_str == "VALUE") {
        attribute_id = UA_ATTRIBUTEID_VALUE;
    } else if (attribute_id_str == "DATATYPE") {
        attribute_id = UA_ATTRIBUTEID_DATATYPE;
    } else if (attribute_id_str == "VALUERANK") {
        attribute_id = UA_ATTRIBUTEID_VALUERANK;
    } else if (attribute_id_str == "ARRAYDIMENSIONS") {
        attribute_id = UA_ATTRIBUTEID_ARRAYDIMENSIONS;
    } else if (attribute_id_str == "ACCESSLEVEL") {
        attribute_id = UA_ATTRIBUTEID_ACCESSLEVEL;
    } else if (attribute_id_str == "USERACCESSLEVEL") {
        attribute_id = UA_ATTRIBUTEID_USERACCESSLEVEL;
    } else if (attribute_id_str == "MINIMUMSAMPLINGINTERVAL") {
        attribute_id = UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL;
    } else if (attribute_id_str == "HISTORIZING") {
        attribute_id = UA_ATTRIBUTEID_HISTORIZING;
    } else if (attribute_id_str == "EXECUTABLE") {
        attribute_id = UA_ATTRIBUTEID_EXECUTABLE;
    } else if (attribute_id_str == "USEREXECUTABLE") {
        attribute_id = UA_ATTRIBUTEID_USEREXECUTABLE;
    } else if (attribute_id_str == "DATATYPEDEFINITION") {
        attribute_id = UA_ATTRIBUTEID_DATATYPEDEFINITION;
    } else {
        RTI_THROW_GATEWAY_EXCEPTION(
                &DDSOPCUA_LOG_OPCUA_UNKNOWN_ATTRIBUTE_s,
                attribute_id_str.c_str());
    }

    return attribute_id;
}

}}}}  // namespace rti::opcua::sdk::types
