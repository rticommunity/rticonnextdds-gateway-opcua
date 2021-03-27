/*
 * (c) 2018-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#ifndef RTI_DDS_OPCUA_CONFIG_XMLTRANSFORMATIONPARAMS_H
#define RTI_DDS_OPCUA_CONFIG_XMLTRANSFORMATIONPARAMS_H

#include <string>
#include <vector>

#include "RtiXmlUtilsDecl.hpp"

namespace rti { namespace ddsopcua { namespace config {

class XmlTransformationParams {
public:
    static const std::vector<RTIXMLUTILSTransformationParameter>&
    transformation_params();

public:
    static const std::string DDSOPCUA_PLUGIN_NAME_PARAM;
    static const std::string DDSOPCUA_PLUGIN_NAME_PROPERTY;

    static const std::string DDSOPCUA_DOMAIN_ROUTE_NAME_PARAM;
    static const std::string DDSOPCUA_DOMAIN_ROUTE_NAME_PROPERTY;

    static const std::string
            DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE_PARAM;
    static const std::string
            DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE_PROPERTY;

    static const std::string
            DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION_PARAM;
    static const std::string
            DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION_PROPERTY;

    static const std::string DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PARAM;
    static const std::string
            DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY;

    static const std::string
            DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PARAM;
    static const std::string
            DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY;

    static const std::string DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PARAM;
    static const std::string DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY;

    static const std::string DDSOPCUA_OPCUA_CONNECTION_FQN_PARAM;
    static const std::string DDSOPCUA_OPCUA_CONNECTION_FQN_PROPERTY;

    static const std::string DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PARAM;
    static const std::string DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY;

private:
    static std::vector<RTIXMLUTILSTransformationParameter>
            transformation_params_;
};

}}}  // namespace rti::ddsopcua::config

#endif  // RTI_DDS_OPCUA_CONFIG_XMLTRANSFORMATIONPARAMS_H
