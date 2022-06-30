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

#ifndef RTI_OPCUASDK_SDKDATATYPES_HPP_
#define RTI_OPCUASDK_SDKDATATYPES_HPP_

#include <string>

// #include <open62541/constants.h>
#include <open62541/types.h>
#include <open62541/types_generated_handling.h>

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

namespace rti { namespace opcua { namespace sdk { namespace types {

static const uint32_t STATUS_CODE_GOOD = UA_STATUSCODE_GOOD;
static const std::string REASON_PHRASE_GOOD = "Good";

class NodeId {
public:
    NodeId() : node_id_(UA_NodeId_new())
    {
        UA_NodeId_init(node_id_);
    }

    NodeId(const UA_NodeId& node_id) : node_id_(UA_NodeId_new())
    {
        UA_NodeId_copy(&node_id, node_id_);
    }

    NodeId(const UA_NodeId* node_id) : node_id_(UA_NodeId_new())
    {
        UA_NodeId_copy(node_id, node_id_);
    }

    NodeId(const NodeId& node_id) : node_id_(UA_NodeId_new())
    {
        UA_NodeId_copy(node_id.node_id_, node_id_);
    }

    NodeId(const uint16_t ns, const uint32_t id) : node_id_(UA_NodeId_new())
    {
        *node_id_ = UA_NODEID_NUMERIC(ns, id);
    }

    NodeId(const uint16_t ns, const std::string& id) : node_id_(UA_NodeId_new())
    {
        *node_id_ = UA_NODEID_STRING_ALLOC(ns, id.c_str());
    }

    NodeId(const uint16_t ns, UA_Guid id) : node_id_(UA_NodeId_new())
    {
        *node_id_ = UA_NODEID_GUID(ns, id);
    }

    ~NodeId()
    {
        if (node_id_ != nullptr) {
            UA_NodeId_delete(node_id_);
        }
    }

    NodeId& operator=(const NodeId& node_id)
    {
        UA_NodeId_copy(node_id.node_id_, node_id_);
        return *this;
    }

    NodeId& operator=(const UA_NodeId& node_id)
    {
        UA_NodeId_copy(&node_id, node_id_);
        return *this;
    }

    const UA_NodeId* get_const_ref() const
    {
        return node_id_;
    }

private:
    UA_NodeId* node_id_;
};

enum class BuiltinTypeKind {
    BOOL_TYPE,
    SBYTE_TYPE,
    BYTE_TYPE,
    INT16_TYPE,
    UINT16_TYPE,
    INT32_TYPE,
    UINT32_TYPE,
    INT64_TYPE,
    UINT64_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    STRING_TYPE,
    DATETIME_TYPE,
    GUID_TYPE,
    BYTESTRING_TYPE,
    XMLELEMENT_TYPE,
    NODEID_TYPE,
    EXPANDEDNODEID_TYPE,
    STATUSCODE_TYPE,
    QUALIFIEDNAME_TYPE,
    LOCALIZEDTEXT_TYPE,
    EXTENSIONOBJECT_TYPE,
    DATAVALUE_TYPE,
    VARIANT_TYPE,
    DIAGNOSTICINFO_TYPE
};


class Variant {
public:
    Variant() : variant_(UA_Variant_new())
    {
        UA_Variant_init(variant_);
    }

    Variant(const UA_Variant& variant) : variant_(UA_Variant_new())
    {
        UA_Variant_copy(&variant, variant_);
    }

    Variant(const UA_Variant* variant) : variant_(UA_Variant_new())
    {
        UA_Variant_copy(variant, variant_);
    }

    Variant(const Variant& variant) : variant_(UA_Variant_new())
    {
        UA_Variant_copy(variant.variant_, variant_);
    }

    ~Variant()
    {
        if (variant_ != nullptr) {
            UA_Variant_clear(variant_);
            UA_Variant_delete(variant_);
        }
    }

    const UA_Variant* get_const_ref() const
    {
        return variant_;
    }

    UA_Variant* get()
    {
        return variant_;
    }

    BuiltinTypeKind get_type_kind() const
    {
        if (variant_->type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
            return BuiltinTypeKind::BOOL_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_SBYTE]) {
            return BuiltinTypeKind::SBYTE_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_BYTE]) {
            return BuiltinTypeKind::BYTE_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_INT16]) {
            return BuiltinTypeKind::INT16_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_UINT16]) {
            return BuiltinTypeKind::UINT16_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_INT32]) {
            return BuiltinTypeKind::INT32_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_UINT32]) {
            return BuiltinTypeKind::UINT32_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_INT64]) {
            return BuiltinTypeKind::INT64_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_UINT64]) {
            return BuiltinTypeKind::UINT64_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_FLOAT]) {
            return BuiltinTypeKind::FLOAT_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_DOUBLE]) {
            return BuiltinTypeKind::DOUBLE_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_STRING]) {
            return BuiltinTypeKind::STRING_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_DATETIME]) {
            return BuiltinTypeKind::DATETIME_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_GUID]) {
            return BuiltinTypeKind::GUID_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_BYTESTRING]) {
            return BuiltinTypeKind::BYTESTRING_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_XMLELEMENT]) {
            return BuiltinTypeKind::XMLELEMENT_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_NODEID]) {
            return BuiltinTypeKind::NODEID_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_EXPANDEDNODEID]) {
            return BuiltinTypeKind::EXPANDEDNODEID_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_STATUSCODE]) {
            return BuiltinTypeKind::STATUSCODE_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_QUALIFIEDNAME]) {
            return BuiltinTypeKind::QUALIFIEDNAME_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]) {
            return BuiltinTypeKind::LOCALIZEDTEXT_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]) {
            return BuiltinTypeKind::EXTENSIONOBJECT_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_DATAVALUE]) {
            return BuiltinTypeKind::DATAVALUE_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_VARIANT]) {
            return BuiltinTypeKind::VARIANT_TYPE;
        } else if (variant_->type == &UA_TYPES[UA_TYPES_DIAGNOSTICINFO]) {
            return BuiltinTypeKind::DIAGNOSTICINFO_TYPE;
        }

        return BuiltinTypeKind::VARIANT_TYPE;
    }

    bool is_scalar()
    {
        return UA_Variant_isScalar(variant_);
    }

private:
    UA_Variant* variant_;
};


class AttributeId {
public:
    AttributeId()
            : attribute_id_(UA_ATTRIBUTEID_VALUE), attribute_id_str_("VALUE")
    {
    }

    AttributeId(const std::string& attribute_id)
            : attribute_id_str_(attribute_id)
    {
        attribute_id_ = attribute_id_string_to_attribute_id(attribute_id);
    }

    AttributeId(const UA_AttributeId attribute_id) : attribute_id_(attribute_id)
    {
        attribute_id_str_ = attribute_id_to_attribute_id_string(attribute_id);
    }

    AttributeId(const uint32_t attribute_id)
            : attribute_id_(static_cast<UA_AttributeId>(attribute_id))
    {
        attribute_id_str_ = attribute_id_to_attribute_id_string(
                static_cast<UA_AttributeId>(attribute_id));
    }

    AttributeId& operator=(const UA_AttributeId& attribute_id)
    {
        attribute_id_ = attribute_id;
        attribute_id_str_ = attribute_id_to_attribute_id_string(attribute_id);

        return *this;
    }

    AttributeId& operator=(const AttributeId& attribute_id)
    {
        attribute_id_ = attribute_id.attribute_id_;
        attribute_id_str_ = attribute_id.attribute_id_str_;

        return *this;
    }

    uint32_t value() const
    {
        return static_cast<uint32_t>(attribute_id_);
    }

    std::string to_string() const
    {
        return attribute_id_str_;
    }

private:
    static std::string attribute_id_to_attribute_id_string(
            const uint32_t attribute_id);

    static UA_AttributeId attribute_id_string_to_attribute_id(
            const std::string& attribute_id_str);

private:
    UA_AttributeId attribute_id_;
    std::string attribute_id_str_;
};


inline std::string node_identifier_to_string(const NodeId& node_id)
{
    if (node_id.get_const_ref()->identifierType == UA_NODEIDTYPE_NUMERIC) {
        return std::to_string(node_id.get_const_ref()->identifier.numeric);
    } else if (
            node_id.get_const_ref()->identifierType == UA_NODEIDTYPE_STRING) {
        return std::string(
                reinterpret_cast<char*>(
                        node_id.get_const_ref()->identifier.string.data),
                node_id.get_const_ref()->identifier.string.length);
    }

    return "";
}


}}}}  // namespace rti::opcua::sdk::types

#endif  // RTI_OPCUASDK_SDKDATATYPES_HPP_
