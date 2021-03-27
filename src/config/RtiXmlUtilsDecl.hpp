/*
 * (c) Copyright, Real-Time Innovations, 2016-2020.
 * All rights reserved.
 *
 * No duplications, whole or partial, manual or electronic, may be made
 * without express written permission.  Any such copies, or
 * revisions thereof, must display this notice unaltered.
 * This code contains trade secrets of Real-Time Innovations, Inc.
 */

#ifndef RTI_DDS_OPCUA_CONFIG_RTIXMLUTILS_DEFINITIONS_
#define RTI_DDS_OPCUA_CONFIG_RTIXMLUTILS_DEFINITIONS_

#include "cdr/cdr_typeCode.h"
#include "log/log_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct RTIXMLUTILSObject;
struct RTIXMLUTILSTransformer;
struct RTIXMLUTILSValidator;

struct RTIXMLUTILSTransformationParameter {
    const char* name;
    const void* value;
    RTICdrTCKind valueKind;
};

struct RTIXMLUTILSProperty {
    char* name;
    char* value;
};

struct RTIXMLUTILSPropertyList {
    struct RTIXMLUTILSProperty* _propertyArray;
    int _propertyArrayLength;
    int _propertyArrayLengthMax;
};

extern struct RTIXMLUTILSTransformer* RTIXMLUTILSTransformer_newFromStringArray(
        const char* stylesheetStringArray[],
        int stylesheetStringArrayLength);

extern struct RTIXMLUTILSObject* RTIXMLUTILSTransformer_transformWithParams(
        struct RTIXMLUTILSTransformer* self,
        struct RTIXMLUTILSObject* element,
        const struct RTIXMLUTILSTransformationParameter* parameterArray,
        int parameterArrayLength);

extern void RTIXMLUTILSTransformer_delete(struct RTIXMLUTILSTransformer* self);

extern struct RTIXMLUTILSValidator* RTIXMLUTILSValidator_newFromStringArray(
        const char* schemaStringArray[],
        int schemaStringArrayLength);

extern RTIBool RTIXMLUTILSValidator_validate(
        struct RTIXMLUTILSValidator* self,
        struct RTIXMLUTILSObject* element);

extern void RTIXMLUTILSValidator_delete(struct RTIXMLUTILSValidator* self);

extern const char* RTIXMLUTILSObject_getFullyQualifiedName(
        struct RTIXMLUTILSObject* self);

extern struct RTIXMLUTILSObject* RTIXMLUTILSObject_lookUp(
        struct RTIXMLUTILSObject* self,
        const char* name);

extern const char* RTIXMLUTILSObject_getText(struct RTIXMLUTILSObject* self);

extern struct RTIXMLUTILSObject* RTIXMLUTILSObject_getFirstChildWithTag(
        struct RTIXMLUTILSObject* self,
        const char* tagName);

extern struct RTIXMLUTILSObject* RTIXMLUTILSObject_getNextSibling(
        struct RTIXMLUTILSObject* self);

extern const char* RTIXMLUTILSObject_getAttribute(
        struct RTIXMLUTILSObject* self,
        const char* attributeName);

extern struct RTIXMLUTILSObject* RTIXMLUTILSObject_getFirstChild(
        struct RTIXMLUTILSObject* self);

extern const char* RTIXMLUTILSObject_getName(struct RTIXMLUTILSObject* self);

extern const char* RTIXMLUTILSObject_getTagName(struct RTIXMLUTILSObject* self);

extern struct RTIXMLUTILSObject* RTIXMLUTILSObject_getNextSiblingWithTag(
        struct RTIXMLUTILSObject* self,
        const char* tagName);

extern const char* RTIXMLUTILSObject_getFilePath(
        struct RTIXMLUTILSObject* self);

extern const char* RTIXMLUTILSObject_getAnnotationDocText(
        struct RTIXMLUTILSObject* self);

extern void RTIXMLUTILSParser_freeDom(struct RTIXMLUTILSObject* element);

extern char* RTIXMLUTILSObject_toString(struct RTIXMLUTILSObject* self);

extern RTIBool RTIXMLUTILSParser_mergeRoot(
        struct RTIXMLUTILSObject* dest,
        struct RTIXMLUTILSObject* from);

extern RTIBool RTIXMLUTILSVariableExpansor_expandFromEnvironmentOrDictionary(
        struct RTIXMLUTILSObject* element,
        struct RTIXMLUTILSPropertyList* dictionary);

extern struct RTIXMLUTILSObject* RTIXMLUTILSObject_copyAsChild(
        struct RTIXMLUTILSObject* self,
        struct RTIXMLUTILSObject* element);

extern RTIBool RTIXMLUTILSParser_parseStringArray(
        struct RTIXMLUTILSObject** root,
        const char* xmlStringArray[],
        int xmlStringArrayLength);

extern RTIBool RTIXMLUTILSParser_parseString(
        struct RTIXMLUTILSObject** root,
        const char* xmlString);

extern RTIBool RTIXMLUTILSParser_parseUrlGroupList(
        struct RTIXMLUTILSObject** root,
        const char* urlGroupList);

extern RTIBool RTIXMLUTILSGlobals_initialize();

extern void RTIXMLUTILSGlobals_finalize();

#ifdef __cplusplus
}
#endif

namespace rti { namespace xmlutils {

class Object {
public:
    Object() : object_(nullptr)
    {
    }

    Object(RTIXMLUTILSObject* object) : object_(object)
    {
    }

    ~Object()
    {
        RTIXMLUTILSParser_freeDom(object_);
    }

    void native(RTIXMLUTILSObject* object)
    {
        object_ = object;
    }

    RTIXMLUTILSObject* native() const
    {
        return object_;
    }

    bool valid()
    {
        return object_ == nullptr ? false : true;
    }

private:
    RTIXMLUTILSObject* object_;
};

class Validator {
public:
    Validator() : validator_(nullptr)
    {
    }

    Validator(RTIXMLUTILSValidator* validator) : validator_(validator)
    {
    }

    ~Validator()
    {
        RTIXMLUTILSValidator_delete(validator_);
    }

    void native(RTIXMLUTILSValidator* validator)
    {
    }

    RTIXMLUTILSValidator* native()
    {
        return validator_;
    }

    bool valid()
    {
        return validator_ == nullptr ? false : true;
    }

private:
    RTIXMLUTILSValidator* validator_;
};


class Transformer {
public:
    Transformer() : transformer_(nullptr)
    {
    }

    Transformer(RTIXMLUTILSTransformer* transformer)
    {
        transformer_ = transformer;
    }

    ~Transformer()
    {
        RTIXMLUTILSTransformer_delete(transformer_);
    }

    void native(RTIXMLUTILSTransformer* transformer)
    {
        transformer_ = transformer;
    }

    RTIXMLUTILSTransformer* native()
    {
        return transformer_;
    }

    bool valid()
    {
        return transformer_ == nullptr ? false : true;
    }

private:
    RTIXMLUTILSTransformer* transformer_;
};

}}  // namespace rti::xmlutils

#endif  // RTI_DDS_OPCUA_CONFIG_RTIXMLUTILS_DEFINITIONS_
