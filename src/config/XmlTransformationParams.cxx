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

#include <dds_c/dds_c_typecode.h>

#include "config/XmlTransformationParams.hpp"

namespace rti { namespace ddsopcua { namespace config {

const std::string XmlTransformationParams::DDSOPCUA_PLUGIN_NAME_PARAM =
        "DDSOPCUA_PLUGIN_NAME";
const std::string XmlTransformationParams::DDSOPCUA_PLUGIN_NAME_PROPERTY =
        "ddsopcua_plugin";

const std::string XmlTransformationParams::DDSOPCUA_DOMAIN_ROUTE_NAME_PARAM =
        "DDSOPCUA_DOMAIN_ROUTE_NAME";
const std::string XmlTransformationParams::DDSOPCUA_DOMAIN_ROUTE_NAME_PROPERTY =
        "ddsopcua_route";

const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE_PARAM =
                "DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE";
const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE_PROPERTY =
                "ddsopcua_processor_attribute";

const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION_PARAM =
                "DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION";
const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION_PROPERTY =
                "ddsopcua_processor_opcua_publication";

const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PARAM =
                "DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY_NAME";
const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY =
                "rti.ddsopcua.opcua2dds.publication.dds_input_fqn";

const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PARAM =
                "DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY_NAME";
const std::string XmlTransformationParams::
        DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY =
                "rti.ddsopcua.opcua2dds.publication.opcua_output_fqn";

const std::string
        XmlTransformationParams::DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PARAM =
                "DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY_NAME";
const std::string
        XmlTransformationParams::DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY =
                "rti.ddsopcua.opcua2dds.subscription_fqn";

const std::string XmlTransformationParams::DDSOPCUA_OPCUA_CONNECTION_FQN_PARAM =
        "DDSOPCUA_OPCUA_CONNECTION_FQN_PROPERTY_NAME";
const std::string
        XmlTransformationParams::DDSOPCUA_OPCUA_CONNECTION_FQN_PROPERTY =
                "rti.ddsopcua.opcua_connection_fqn";

const std::string
        XmlTransformationParams::DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PARAM =
                "DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY_NAME";
const std::string
        XmlTransformationParams::DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY =
                "rti.ddsopcua.opcua_connection.server_endpoint_url";


std::vector<RTIXMLUTILSTransformationParameter>
        XmlTransformationParams::transformation_params_;

const std::vector<RTIXMLUTILSTransformationParameter>& XmlTransformationParams::
        transformation_params()
{
    RTIXMLUTILSTransformationParameter parameter;

    if (transformation_params_.empty()) {
        // DDSOPCUA_PLUGIN_NAME
        parameter.name = DDSOPCUA_PLUGIN_NAME_PARAM.c_str();
        parameter.value = DDSOPCUA_PLUGIN_NAME_PROPERTY.c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_DOMAIN_ROUTE_NAME
        parameter.name = DDSOPCUA_DOMAIN_ROUTE_NAME_PARAM.c_str();
        parameter.value = DDSOPCUA_DOMAIN_ROUTE_NAME_PROPERTY.c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE
        parameter.name =
                DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE_PARAM.c_str();
        parameter.value =
                DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE_PROPERTY
                        .c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE
        parameter.name =
                DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION_PARAM.c_str();
        parameter.value =
                DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION_PROPERTY
                        .c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY_NAME
        parameter.name =
                DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PARAM.c_str();
        parameter.value =
                DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY.c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY_NAME
        parameter.name =
                DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PARAM.c_str();
        parameter.value =
                DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY
                        .c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY_NAME
        parameter.name = DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PARAM.c_str();
        parameter.value = DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY.c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_OPCUA_CONNECTION_FQN_PROPERTY_NAME
        parameter.name = DDSOPCUA_OPCUA_CONNECTION_FQN_PARAM.c_str();
        parameter.value = DDSOPCUA_OPCUA_CONNECTION_FQN_PROPERTY.c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);

        // DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY_NAME
        parameter.name = DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PARAM.c_str();
        parameter.value = DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY.c_str();
        parameter.valueKind = static_cast<RTICdrTCKind>(DDS_TK_STRING);
        transformation_params_.push_back(parameter);
    }

    return transformation_params_;
}

}}}  // namespace rti::ddsopcua::config
