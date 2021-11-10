<?xml version="1.0" encoding="UTF-8"?>
<!--
   (c) 2017-2020 Copyright, Real-Time Innovations, Inc. (RTI)
   All rights reserved.

   RTI grants Licensee a license to use, modify, compile, and create derivative
   works of the Software solely in combination with RTI Connext DDS. Licensee
   may redistribute copies of the Software provided that all such copies are
   subject to this License. The Software is provided "as is", with no warranty
   of any type, including any warranty for fitness for any purpose. RTI is
   under no obligation to maintain or support the Software. RTI shall not be
   liable for any incidental or consequential damages arising out of the use or
   inability to use the Software. For purposes of clarity, nothing in this
   License prevents Licensee from using alternate versions of DDS, provided
   that Licensee may not combine or link such alternate versions of DDS with
   the Software.
-->

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:exslt="http://xmlsoft.org/XSLT/namespace"
    xmlns:saxon="http://icl.com/saxon"
    version="1.0">

    <xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>

    <!--
        =============================================================
                            Global Variables
        =============================================================
    -->

    <xsl:variable name="NAME_FQN_SEPARATOR"
                  select="'::'" />
    <xsl:variable name="RTI_MODULE_NAME"
                  select="'__rti'" />
    <xsl:variable name="DDSOPCUA_MODULE_NAME"
                  select="'ddsopcua'" />
    <xsl:variable name="TYPES_DUMMY_TYPE_NAME"
                  select="'Stub'" />
    <xsl:variable name="TYPES_DUMMY_TYPE_FQN"
                  select="concat(
                    $RTI_MODULE_NAME,
                    $NAME_FQN_SEPARATOR,
                    $DDSOPCUA_MODULE_NAME,
                    $NAME_FQN_SEPARATOR,
                    $TYPES_DUMMY_TYPE_NAME)" />
    <xsl:variable name="TYPES_DUMMY_TYPE_REGISTERED_NAME"
                  select="concat(
                    $RTI_MODULE_NAME,
                    '_',
                    $DDSOPCUA_MODULE_NAME,
                    '_',
                    $TYPES_DUMMY_TYPE_NAME)" />
    <xsl:variable name="ATTRIBUTE_SERVICE_SET_REQUEST_TYPE"
                  select="'rti::opcua::types::services::ServiceRequest'" />
    <xsl:variable name="ATTRIBUTE_SERVICE_SET_REPLY_TYPE"
                  select="'rti::opcua::types::services::ServiceReply'" />
    <xsl:variable name="ATTRIBUTE_SERVICE_SET_REQUEST_TOPIC_NAME"
                  select="'AttributeServiceSet_Request'"/>
    <xsl:variable name="ATTRIBUTE_SERVICE_SET_REPLY_TOPIC_NAME"
                  select="'AttributeServiceSet_Reply'"/>
    <xsl:variable name="OPCUA_CLIENT_STREAM_NAME"
                  select="'OpcUaClient'"/>
    <xsl:variable name="DDS_QOS_PROFILE_TRANSIENT_LOCAL_KEEP_LAST"
                  select="'BuiltinQosLibExp::Generic.KeepLastReliable.TransientLocal'"/>

    <!--
        =============================================================
                            Input Parameters
        =============================================================
    -->

    <xsl:param name="DDSOPCUA_PLUGIN_NAME"/>
    <xsl:param name="DDSOPCUA_DOMAIN_ROUTE_NAME"/>
    <xsl:param name="DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE"/>
    <xsl:param name="DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION"/>
    <xsl:param name="DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY_NAME"/>
    <xsl:param name="DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY_NAME"/>
    <xsl:param name="DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY_NAME"/>
    <xsl:param name="DDSOPCUA_OPCUA_CONNECTION_SERVER_FQN_PROPERTY_NAME"/>
    <xsl:param name="DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY_NAME"/>

    <!--
        =============================================================
                            Utility templates
        =============================================================
    -->

    <!--
        Template: Generate the line information for a XML element.
    -->
    <xsl:template name="GenerateLineInfo">
        <xsl:param name="nodeParam" select="."/>
        <xsl:choose>
            <xsl:when test="not($nodeParam)">
                <!-- No arguments = end of fragment, reset context line counter -->
                <xsl:processing-instruction name="rti-linenumber">
                    <xsl:text>END-OF-XML-FRAGMENT</xsl:text>
                </xsl:processing-instruction>
            </xsl:when>
            <xsl:otherwise>
                <xsl:processing-instruction name="rti-linenumber">
                    <xsl:value-of select="saxon:line-number($nodeParam)"/>
                </xsl:processing-instruction>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="DummyTypeModule">
        <xsl:element name="module">
            <xsl:attribute name="name">
                <xsl:value-of select="$RTI_MODULE_NAME" />
            </xsl:attribute>
            <xsl:element name="module">
                <xsl:attribute name="name">
                    <xsl:value-of select="$DDSOPCUA_MODULE_NAME" />
                </xsl:attribute>
                <xsl:element name="struct">
                    <xsl:attribute name="name">
                        <xsl:value-of select="$TYPES_DUMMY_TYPE_NAME" />
                    </xsl:attribute>
                    <xsl:element name="member">
                        <xsl:attribute name="name">
                            <xsl:text>stub</xsl:text>
                        </xsl:attribute>
                        <xsl:attribute name="type">
                            <xsl:text>octet</xsl:text>
                        </xsl:attribute>
                    </xsl:element>
                </xsl:element>
            </xsl:element>
        </xsl:element>
    </xsl:template>
    <!--
        =============================================================
                       Transformation templates
        =============================================================
    -->

    <!--
      Template: Generates <dds> tag
    -->
    <xsl:template name="DdsRoot" match="dds">
        <xsl:element name="dds">
            <xsl:copy-of select="@*"/>
            <xsl:element name="types">
                <xsl:call-template name="DummyTypeModule"/>
                <xsl:for-each select="./types">
                    <xsl:copy-of select="./*"/>
                </xsl:for-each>
            </xsl:element>
            <xsl:copy-of select="qos_library"/>
            <xsl:apply-templates select="ddsopcua_service" />
        </xsl:element>
    </xsl:template>

    <xsl:template name="ApplyRegisteredType">
        <xsl:param name="dds_participant_name" />
        <xsl:param name="dds_registered_type_name"/>
        <xsl:element name="registered_type">
            <xsl:attribute name="name">
                <xsl:value-of select="$dds_registered_type_name"/>
            </xsl:attribute>
            <xsl:attribute name="type_name">
                <xsl:value-of select="../../domain_participant[@name = $dds_participant_name]/register_type[@name = $dds_registered_type_name]/@type_ref"/>
            </xsl:attribute>
        </xsl:element>
    </xsl:template>
    <xsl:template name="AddRegisteredType"
                  match="publication | subscription">
        <xsl:param name="opcua_server_name"/>
        <xsl:variable name="dds_participant_name">
            <xsl:choose>
                <xsl:when test="./opcua_input/@opcua_connection_ref = $opcua_server_name">
                    <xsl:value-of select="./dds_output/@domain_participant_ref" />
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="./dds_input/@domain_participant_ref" />
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:variable name="dds_registered_type_name">
            <xsl:choose>
                <xsl:when test="./opcua_input/@opcua_connection_ref = $opcua_server_name">
                    <xsl:value-of select="./dds_output/registered_type_name"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="./dds_input/registered_type_name"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <!-- Only register a type within a specific connection once -->
        <xsl:if test="(count(preceding-sibling::subscription/dds_output[registered_type_name=$dds_registered_type_name]) = 0)
                      and (count(preceding-sibling::publication/dds_input[registered_type_name=$dds_registered_type_name]) = 0)">
            <xsl:call-template name="ApplyRegisteredType">
                <xsl:with-param name="dds_participant_name" select="$dds_participant_name"/>
                <xsl:with-param name="dds_registered_type_name" select="$dds_registered_type_name"/>
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <!--
      Template: Generates <connection> tags
    -->
    <xsl:template name="OpcUaServerConnection">

        <xsl:param name="service_name"/>

        <xsl:for-each select="./opcua_connection">

            <xsl:variable name="opcua_connection_name"
                          select="./@name"/>

            <xsl:element name="connection">
                <xsl:variable name="opcua_server_name" select="./@name" />

                <xsl:attribute name="name">
                    <xsl:value-of select="./@name" />
                </xsl:attribute>

                <xsl:attribute name="plugin_name">
                    <xsl:value-of select="$DDSOPCUA_PLUGIN_NAME" />
                </xsl:attribute>

                <xsl:element name="property">
                    <xsl:element name="value">
                        <xsl:element name="element">
                            <xsl:element name="name">
                                <xsl:value-of select="$DDSOPCUA_OPCUA_CONNECTION_SERVER_URL_PROPERTY_NAME"/>
                            </xsl:element>
                            <xsl:element name="value">
                                <xsl:value-of select="./@server_endpoint_url"/>
                            </xsl:element>
                        </xsl:element>
                        <xsl:element name="element">
                            <xsl:element name="name">
                                <xsl:value-of select="$DDSOPCUA_OPCUA_CONNECTION_FQN_PROPERTY_NAME"/>
                            </xsl:element>
                            <xsl:element name="value">
                                <xsl:value-of select="concat(
                                        $service_name,
                                        $NAME_FQN_SEPARATOR,
                                        $opcua_connection_name)"/>
                            </xsl:element>
                        </xsl:element>
                    </xsl:element>
                </xsl:element>

                <xsl:for-each select="../opcua_to_dds_bridge">
                    <xsl:apply-templates select="publication|subscription">
                        <xsl:with-param name="opcua_server_name" select="$opcua_server_name"/>
                    </xsl:apply-templates>
                </xsl:for-each>
                <xsl:element name="registered_type">
                    <xsl:attribute name="name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TYPE" />
                    </xsl:attribute>
                    <xsl:attribute name="type_name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TYPE" />
                    </xsl:attribute>
                </xsl:element>
                <xsl:element name="registered_type">
                    <xsl:attribute name="name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TYPE" />
                    </xsl:attribute>
                    <xsl:attribute name="type_name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TYPE" />
                    </xsl:attribute>
                </xsl:element>
            </xsl:element>

        </xsl:for-each>

    </xsl:template>

    <!--
        Template: Generates <participant> tag
    -->
    <xsl:template name="DdsDomainParticipant">

        <xsl:for-each select="./domain_participant">

            <xsl:variable name="dds_domain_participant_name"
                          select="./@name"/>

            <xsl:element name="participant">
                <xsl:attribute name="name">
                    <xsl:value-of select="$dds_domain_participant_name" />
                </xsl:attribute>
                <xsl:element name="domain_id">
                    <xsl:choose>
                        <xsl:when test="./@domain_id">
                            <xsl:value-of select="./@domain_id"/>
                        </xsl:when>
                        <!-- If the domain id was not specified, then
                             set <domain_id>0</domain_id> -->
                        <xsl:otherwise>
                            <xsl:text>0</xsl:text>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:element>
                <xsl:copy-of select="participant_qos"/>
                <xsl:for-each select="register_type">
                    <xsl:element name="registered_type">
                        <xsl:attribute name="name">
                            <xsl:value-of select="./@name" />
                        </xsl:attribute>
                        <xsl:attribute name="type_name">
                            <xsl:value-of select="./@type_ref" />
                        </xsl:attribute>
                    </xsl:element>
                </xsl:for-each>
                <xsl:element name="registered_type">
                    <xsl:attribute name="name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TYPE" />
                    </xsl:attribute>
                    <xsl:attribute name="type_name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TYPE" />
                    </xsl:attribute>
                </xsl:element>
                <xsl:element name="registered_type">
                    <xsl:attribute name="name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TYPE" />
                    </xsl:attribute>
                    <xsl:attribute name="type_name">
                        <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TYPE" />
                    </xsl:attribute>
                </xsl:element>
            </xsl:element>
        </xsl:for-each>
    </xsl:template>

    <xsl:template name="OpcUaInput">
        <xsl:param name="service_name" />
        <xsl:param name="opcua_to_dds_bridge_name" />
        <xsl:param name="subscription_name" />
        <xsl:param name="dds_output_type_name" />
        <xsl:element name="input">
            <xsl:attribute name="name">
                <xsl:value-of select="./@name" />
            </xsl:attribute>
            <xsl:attribute name="connection">
                <xsl:value-of select="./@opcua_connection_ref" />
            </xsl:attribute>
            <xsl:element name="creation_mode">
                <xsl:text>IMMEDIATE</xsl:text>
            </xsl:element>
            <xsl:element name="stream_name">
                <xsl:value-of select="./@name" />
            </xsl:element>
            <xsl:element name="registered_type_name">
                <xsl:value-of select="$dds_output_type_name" />
                <!-- <xsl:value-of select="$TYPES_DUMMY_TYPE_REGISTERED_NAME" /> -->
            </xsl:element>
            <xsl:element name="property">
                <xsl:element name="value">
                    <xsl:element name="element">
                        <xsl:element name="name">
                            <xsl:value-of select="$DDSOPCUA_OPCUA2DDS_SUBSCRIPTION_FQN_PROPERTY_NAME" />
                        </xsl:element>
                        <xsl:element name="value">
                            <xsl:value-of select="concat(
                                    $service_name,
                                    $NAME_FQN_SEPARATOR,
                                    $opcua_to_dds_bridge_name,
                                    $NAME_FQN_SEPARATOR,
                                    $subscription_name,
                                    $NAME_FQN_SEPARATOR,
                                    ./@name)" />
                        </xsl:element>
                    </xsl:element>
                </xsl:element>
            </xsl:element>
        </xsl:element>
    </xsl:template>

    <xsl:template name="OpcUaOutput">
        <xsl:param name="service_name" />
        <xsl:param name="opcua_to_dds_bridge_name" />
        <xsl:param name="publication_name" />
        <xsl:param name="dds_input_type_name" />

        <xsl:element name="input">
            <xsl:attribute name="name">
                <xsl:value-of select="'OpcUaAttributeServiceSet_Reply'" />
            </xsl:attribute>
            <xsl:attribute name="connection">
                <xsl:value-of select="./@opcua_connection_ref" />
            </xsl:attribute>
            <xsl:element name="creation_mode">
                <xsl:text>IMMEDIATE</xsl:text>
            </xsl:element>
            <xsl:element name="stream_name">
                <xsl:value-of select="'OpcUaAttributeServiceSet_Reply'" />
            </xsl:element>
            <xsl:element name="registered_type_name">
                <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TYPE"/>
            </xsl:element>
        </xsl:element>

        <!-- Output channel to use an OPC UA client -->
        <xsl:element name="output">
            <xsl:attribute name="name">
                <xsl:value-of select="'OpcUaAttributeServiceSet_Request'" />
            </xsl:attribute>
            <xsl:attribute name="connection">
                <xsl:value-of select="./@opcua_connection_ref" />
            </xsl:attribute>
            <xsl:element name="creation_mode">
                <xsl:text>IMMEDIATE</xsl:text>
            </xsl:element>
            <xsl:element name="stream_name">
                <xsl:value-of select="'OpcUaAttributeServiceSet_Request'" />
            </xsl:element>
            <xsl:element name="registered_type_name">
                <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TYPE"/>
            </xsl:element>
        </xsl:element>
    </xsl:template>

    <xsl:template name="DdsInput">
        <xsl:element name="dds_input">
            <xsl:attribute name="name">
                <xsl:value-of select="./@name" />
            </xsl:attribute>
            <xsl:attribute name="participant">
                <xsl:value-of select="./@domain_participant_ref"/>
            </xsl:attribute>

            <xsl:element name="creation_mode">
                <xsl:text>IMMEDIATE</xsl:text>
            </xsl:element>
            <xsl:copy-of select="./topic_name"/>
            <xsl:copy-of select="./registered_type_name"/>
            <xsl:copy-of select="./datawriter_qos"/>
        </xsl:element>
    </xsl:template>

    <xsl:template name="DdsOutput">
        <xsl:element name="dds_output">
            <xsl:attribute name="name">
                <xsl:value-of select="./@name" />
            </xsl:attribute>
            <xsl:attribute name="participant">
                <xsl:value-of select="./@domain_participant_ref"/>
            </xsl:attribute>

            <xsl:element name="creation_mode">
                <xsl:text>IMMEDIATE</xsl:text>
            </xsl:element>
            <xsl:copy-of select="./topic_name"/>
            <xsl:copy-of select="./registered_type_name"/>
            <xsl:copy-of select="./datawriter_qos"/>
        </xsl:element>
    </xsl:template>


    <xsl:template name="AttributeServiceSet">
        <xsl:param name="participant_name" />
        <xsl:param name="opcua_connection_name" />

        <xsl:element name="route">
            <xsl:attribute name="name">
                <xsl:text>ServiceSet_Attribute</xsl:text>
            </xsl:attribute>
            <xsl:element name="publish_with_original_timestamp">
                <xsl:value-of select="1"/>
            </xsl:element>
            <xsl:element name="publish_with_original_info">
                <xsl:value-of select="1"/>
            </xsl:element>
            <xsl:element name="processor">
                <xsl:attribute name="plugin_name">
                    <xsl:value-of select="$DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_ATTRIBUTE_SERVICE" />
                </xsl:attribute>
            </xsl:element>
            <xsl:element name="dds_input">
                <xsl:attribute name="name">
                    <xsl:value-of select="'DdsAttributeServiceSet_Request'" />
                </xsl:attribute>
                <xsl:attribute name="participant">
                    <xsl:value-of select="$participant_name" />
                </xsl:attribute>
                <xsl:element name="creation_mode">
                    <xsl:text>IMMEDIATE</xsl:text>
                </xsl:element>
                <xsl:element name="topic_name">
                    <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TOPIC_NAME"/>
                </xsl:element>
                <xsl:element name="registered_type_name">
                    <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TYPE"/>
                </xsl:element>
                <xsl:element name="datareader_qos">
                    <xsl:attribute name="base_name">
                        <xsl:value-of select="$DDS_QOS_PROFILE_TRANSIENT_LOCAL_KEEP_LAST" />
                    </xsl:attribute>
                </xsl:element>
            </xsl:element>
            <xsl:element name="dds_output">
                <xsl:attribute name="name">
                    <xsl:value-of select="'DdsAttributeServiceSet_Reply'" />
                </xsl:attribute>
                <xsl:attribute name="participant">
                    <xsl:value-of select="$participant_name" />
                </xsl:attribute>
                <xsl:element name="creation_mode">
                    <xsl:text>IMMEDIATE</xsl:text>
                </xsl:element>
                <xsl:element name="topic_name">
                    <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TOPIC_NAME"/>
                </xsl:element>
                <xsl:element name="registered_type_name">
                    <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TYPE"/>
                </xsl:element>
                <xsl:element name="datawriter_qos">
                    <xsl:attribute name="base_name">
                        <xsl:value-of select="$DDS_QOS_PROFILE_TRANSIENT_LOCAL_KEEP_LAST" />
                    </xsl:attribute>
                </xsl:element>
            </xsl:element>
            <xsl:element name="input">
                <xsl:attribute name="name">
                    <xsl:value-of select="'OpcUaAttributeServiceSet_Reply'" />
                </xsl:attribute>
                <xsl:attribute name="connection">
                    <xsl:value-of select="$opcua_connection_name" />
                </xsl:attribute>
                <xsl:element name="creation_mode">
                    <xsl:text>IMMEDIATE</xsl:text>
                </xsl:element>
                <xsl:element name="stream_name">
                    <xsl:value-of select="'OpcUaAttributeServiceSet_Reply'" />
                </xsl:element>
                <xsl:element name="registered_type_name">
                    <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REPLY_TYPE"/>
                </xsl:element>
            </xsl:element>
            <!-- Output channel to use an OPC UA client -->
            <xsl:element name="output">
                <xsl:attribute name="name">
                    <xsl:value-of select="'OpcUaAttributeServiceSet_Request'" />
                </xsl:attribute>
                <xsl:attribute name="connection">
                    <xsl:value-of select="$opcua_connection_name" />
                </xsl:attribute>
                <xsl:element name="creation_mode">
                    <xsl:text>IMMEDIATE</xsl:text>
                </xsl:element>
                <xsl:element name="stream_name">
                    <xsl:value-of select="'OpcUaAttributeServiceSet_Request'" />
                </xsl:element>
                <xsl:element name="registered_type_name">
                    <xsl:value-of select="$ATTRIBUTE_SERVICE_SET_REQUEST_TYPE"/>
                </xsl:element>
            </xsl:element>
        </xsl:element>
    </xsl:template>

    <xsl:template name="ServiceSets">
        <!-- attribute_service_set -->
        <xsl:if test="(./attribute_service_set/enabled = 'true') or (./attribute_service_set/enabled = '1')">
            <xsl:call-template name="AttributeServiceSet">
                <xsl:with-param name="opcua_connection_name"
                                select="./@opcua_connection_ref"/>
                <xsl:with-param name="participant_name"
                                select="./@domain_participant_ref" />
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <!--
        Template: Generates <route> tags
    -->
    <xsl:template name="DdsOpcUaRoutes">
        <xsl:param name="service_name"/>
        <xsl:param name="opcua_to_dds_bridge_name" />

        <xsl:for-each select="./service_set">
             <xsl:call-template name="ServiceSets" />
        </xsl:for-each>

        <xsl:for-each select="./subscription">
            <xsl:variable name="subscription_name"
                          select="./@name"/>

            <xsl:element name="route">
                <xsl:attribute name="name">
                    <xsl:value-of select="./@name" />
                </xsl:attribute>
                <xsl:element name="publish_with_original_timestamp">
                    <xsl:value-of select="1"/>
                </xsl:element>
                <xsl:element name="publish_with_original_info">
                    <xsl:value-of select="1"/>
                </xsl:element>
                <xsl:for-each select="opcua_input">
                    <xsl:call-template name="OpcUaInput" >
                        <xsl:with-param name="service_name"
                                        select="$service_name"/>
                        <xsl:with-param name="opcua_to_dds_bridge_name"
                                        select="$opcua_to_dds_bridge_name" />
                        <xsl:with-param name="subscription_name"
                                        select="$subscription_name" />
                        <xsl:with-param name="dds_output_type_name"
                                        select="../dds_output/registered_type_name" />
                    </xsl:call-template>
                </xsl:for-each>
                <xsl:for-each select="dds_output">
                    <xsl:call-template name="DdsOutput"/>
                </xsl:for-each>
            </xsl:element>
        </xsl:for-each>

        <xsl:for-each select="./publication">
            <xsl:variable name="publication_name"
                          select="./@name"/>
            <xsl:element name="route">
                <xsl:attribute name="name">
                    <xsl:value-of select="./@name" />
                </xsl:attribute>
                <xsl:element name="publish_with_original_timestamp">
                    <xsl:value-of select="1"/>
                </xsl:element>
                <xsl:element name="publish_with_original_info">
                    <xsl:value-of select="1"/>
                </xsl:element>

                <xsl:for-each select="dds_input">
                    <xsl:call-template name="DdsInput"/>
                </xsl:for-each>

                <xsl:for-each select="opcua_output">
                    <xsl:call-template name="OpcUaOutput" >
                        <xsl:with-param name="service_name"
                                        select="$service_name"/>
                        <xsl:with-param name="opcua_to_dds_bridge_name"
                                        select="$opcua_to_dds_bridge_name" />
                        <xsl:with-param name="publication_name"
                                        select="$publication_name" />
                        <xsl:with-param name="dds_input_type_name"
                                        select="../dds_input/registered_type_name" />
                    </xsl:call-template>
                </xsl:for-each>

                <xsl:element name="processor">
                    <xsl:attribute name="plugin_name">
                        <xsl:value-of select="$DDSOPCUA_OPCUA2DDS_PLUGIN_NAME_OPCUA_PUBLICATION" />
                    </xsl:attribute>
                    <xsl:element name="property">
                        <xsl:element name="value">
                            <xsl:element name="element">
                                <xsl:element name="name">
                                    <xsl:value-of select="$DDSOPCUA_OPCUA2DDS_PUBLICATION_OPCUA_OUTPUT_FQN_PROPERTY_NAME" />
                                </xsl:element>
                                <xsl:element name="value">
                                    <xsl:value-of select="concat(
                                            $service_name,
                                            $NAME_FQN_SEPARATOR,
                                            $opcua_to_dds_bridge_name,
                                            $NAME_FQN_SEPARATOR,
                                            $publication_name,
                                            $NAME_FQN_SEPARATOR,
                                            ./opcua_output/@name)"/>
                                </xsl:element>
                            </xsl:element>
                            <xsl:element name="element">
                                <xsl:element name="name">
                                    <xsl:value-of select="$DDSOPCUA_OPCUA2DDS_PUBLICATION_DDS_INPUT_FQN_PROPERTY_NAME" />
                                </xsl:element>
                                <xsl:element name="value">
                                    <xsl:value-of select="concat(
                                            $service_name,
                                            $NAME_FQN_SEPARATOR,
                                            $opcua_to_dds_bridge_name,
                                            $NAME_FQN_SEPARATOR,
                                            $publication_name,
                                            $NAME_FQN_SEPARATOR,
                                            ./dds_input/@name)"/>
                                </xsl:element>
                            </xsl:element>
                        </xsl:element>
                    </xsl:element>
                </xsl:element>

            </xsl:element>
        </xsl:for-each>
    </xsl:template>

    <!--
       Template: Generates <session> tag
    -->
    <xsl:template name="DdsOpcUaSession">
        <xsl:param name="service_name"/>

        <xsl:for-each select="./opcua_to_dds_bridge">
            <xsl:element name="session">
                <xsl:attribute name="name">
                    <xsl:value-of select="./@name" />
                </xsl:attribute>
                <xsl:copy-of select="publisher_qos"/>
                <xsl:copy-of select="subscriber_qos"/>
                <xsl:call-template name="DdsOpcUaRoutes" >
                    <xsl:with-param name="service_name"
                                    select="$service_name" />
                    <xsl:with-param name="opcua_to_dds_bridge_name"
                                    select="./@name" />
                </xsl:call-template>
            </xsl:element>
        </xsl:for-each>
    </xsl:template>

    <!--
      Template: Generates <ddsopcua_service> tag
    -->
    <xsl:template name="DdsOpcUaService"
                  match="ddsopcua_service">

        <xsl:element name="routing_service">
            <xsl:variable name="routing_service_name"
                          select="./@name"/>

            <xsl:attribute name="name">
                <xsl:value-of select="$routing_service_name" />
            </xsl:attribute>

            <xsl:copy-of select="annotation"/>

            <xsl:element name="domain_route">
                <!-- TODO: This should eventually become a variable -->
                <xsl:attribute name="name">
                    <xsl:value-of select="$DDSOPCUA_DOMAIN_ROUTE_NAME"/>
                </xsl:attribute>

                <xsl:call-template name="OpcUaServerConnection">
                    <xsl:with-param name="service_name"
                                    select="$routing_service_name"/>
                </xsl:call-template>
                <xsl:call-template name="DdsDomainParticipant" />
                <xsl:call-template name="DdsOpcUaSession">
                    <xsl:with-param name="service_name"
                                    select="$routing_service_name"/>
                </xsl:call-template>

            </xsl:element>

        </xsl:element>

    </xsl:template>

</xsl:stylesheet>
