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

#ifndef RTI_DDSOPCUA_GENERIC_REQUESTER_CONVERSIONS_HPP_
#define RTI_DDSOPCUA_GENERIC_REQUESTER_CONVERSIONS_HPP_

#include "conversion/OpcUaBuiltinDataTypes.hpp"

namespace rti { namespace ddsopcua { namespace conversions {

/**
 * @brief Transform a string representing a Node ID into a proper Node ID
 * object that can be used by the underlying infrastructure. The string
 * represent the NodeId according to the following pattern:
 * <namespace_index>:<node_identifier>. Where <namespace_index> must be
 * the string representation of the appropriate numeric namespace index,
 * and <node_identifier> must be the string representation of a numeric or
 * string identifier. Other types of node identifiers are unsupported.
 *
 * @param node_id Output parameter with the transformed NodeId object.
 * @param node_id_str Input parameter with the string containing the node
 * identifier in the format "<namespace_index>:<node_identifier>.
 * @return true String was transformed succesfully.
 * @return false String could not be transformed.
 */
bool string_to_node_id(
        rti::opcua::types::NodeId& node_id,
        const std::string& node_id_str)
{
    size_t separator = node_id_str.find(":");
    if (separator == std::string::npos) {
        std::cerr << "Error: invalid NodeId format." << std::endl;
        return false;
    }

    std::string namespace_index_str = node_id_str.substr(0, separator);

    // Set NamespaceIndex
    char* end_ptr;
    long namespace_index = strtol(namespace_index_str.c_str(), &end_ptr, 10);
    if (*end_ptr != 0) {
        std::cerr << "Error: namespace index must have an integer value\n.";
        return false;
    }
    node_id.namespace_index(static_cast<uint16_t>(namespace_index));

    std::string identifier_str = node_id_str.substr(separator + 1);
    // Set Identifier
    long numeric_identifier = strtol(identifier_str.c_str(), &end_ptr, 10);
    if (*end_ptr != 0) {
        node_id.identifier_type().string_id(identifier_str);
    } else {
        node_id.identifier_type().numeric_id(numeric_identifier);
    }

    return true;
}

bool string_to_variant(
        rti::opcua::types::Variant& variant,
        const rti::opcua::types::BuiltinTypeKind variant_type,
        const std::string& variant_value_str)
{
    const uint32_t scalar_length = 1;
    char* end_ptr;
    switch (variant_type) {
    case rti::opcua::types::BuiltinTypeKind::BOOLEAN_TYPE: {
        if (variant_value_str != "true" && variant_value_str != "false") {
            std::cerr << "Error: invalid boolean value " << variant_value_str
                      << " valid values are 'true' or 'false'" << std::endl;
            return false;
        }
        std::vector<bool> value(
                scalar_length,
                variant_value_str == "true" ? true : false);
        variant.value().bool_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::INT16_TYPE: {
        int16_t scalar_value = static_cast<int16_t>(
                strtol(variant_value_str.c_str(), &end_ptr, 10));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid int16_t value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<int16_t> value(scalar_length, scalar_value);
        variant.value().int16_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::UINT16_TYPE: {
        uint16_t scalar_value = static_cast<uint16_t>(
                strtoul(variant_value_str.c_str(), &end_ptr, 10));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid uint16_t value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<uint16_t> value(scalar_length, scalar_value);
        variant.value().uint16_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::INT32_TYPE: {
        int32_t scalar_value = static_cast<int32_t>(
                strtol(variant_value_str.c_str(), &end_ptr, 10));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid int32_t value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<int32_t> value(scalar_length, scalar_value);
        variant.value().int32_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::UINT32_TYPE: {
        uint32_t scalar_value = static_cast<uint32_t>(
                strtoul(variant_value_str.c_str(), &end_ptr, 10));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid uint32_t value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<uint32_t> value(scalar_length, scalar_value);
        variant.value().uint32_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::INT64_TYPE: {
        int64_t scalar_value = static_cast<int64_t>(
                strtoll(variant_value_str.c_str(), &end_ptr, 10));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid int64_t value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<int64_t> value(scalar_length, scalar_value);
        variant.value().int64_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::UINT64_TYPE: {
        uint64_t scalar_value = static_cast<uint64_t>(
                strtoull(variant_value_str.c_str(), &end_ptr, 10));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid uint64_t value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<uint64_t> value(scalar_length, scalar_value);
        variant.value().uint64_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::FLOAT_TYPE: {
        float scalar_value =
                static_cast<float>(strtof(variant_value_str.c_str(), &end_ptr));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid float value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<float> value(scalar_length, scalar_value);
        variant.value().float_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::DOUBLE_TYPE: {
        double scalar_value = static_cast<double>(
                strtod(variant_value_str.c_str(), &end_ptr));
        if (*end_ptr != 0) {
            std::cerr << "Error: invalid double value " << variant_value_str
                      << std::endl;
            return false;
        }
        std::vector<double> value(scalar_length, scalar_value);
        variant.value().double_value(value);
    } break;
    case rti::opcua::types::BuiltinTypeKind::STRING_TYPE: {
        std::vector<std::string> value(scalar_length, variant_value_str);
        variant.value().string_value(value);
    } break;
    default: {
        std::cerr << "Error: Unsupported type " << variant_type << std::endl;
        return false;
    }
    }

    return true;
}

rti::opcua::types::BuiltinTypeKind string_to_opc_ua_type(
        const std::string& type_str)
{
    if (type_str == "boolean") {
        return rti::opcua::types::BuiltinTypeKind::BOOLEAN_TYPE;
    } else if (type_str == "int16") {
        return rti::opcua::types::BuiltinTypeKind::INT16_TYPE;
    } else if (type_str == "uint16") {
        return rti::opcua::types::BuiltinTypeKind::UINT16_TYPE;
    } else if (type_str == "int32") {
        return rti::opcua::types::BuiltinTypeKind::INT32_TYPE;
    } else if (type_str == "uint32") {
        return rti::opcua::types::BuiltinTypeKind::UINT32_TYPE;
    } else if (type_str == "int64") {
        return rti::opcua::types::BuiltinTypeKind::INT64_TYPE;
    } else if (type_str == "uint64") {
        return rti::opcua::types::BuiltinTypeKind::UINT64_TYPE;
    } else if (type_str == "float") {
        return rti::opcua::types::BuiltinTypeKind::FLOAT_TYPE;
    } else if (type_str == "double") {
        return rti::opcua::types::BuiltinTypeKind::DOUBLE_TYPE;
    } else if (type_str == "string") {
        return rti::opcua::types::BuiltinTypeKind::STRING_TYPE;
    }

    std::cerr << "Error: unsupported type " << type_str << std::endl;
    std::cerr << "Supported types include: boolean, "
                 "int16, uint16, int32, uint32, int64, uint64, float, double, "
                 "and string"
              << std::endl;

    return rti::opcua::types::BuiltinTypeKind::DIAGNOSTICINFO_TYPE;
}

}}}  // namespace rti::ddsopcua::conversions

#endif  // RTI_DDSOPCUA_GENERIC_REQUESTER_CONVERSIONS_HPP_
