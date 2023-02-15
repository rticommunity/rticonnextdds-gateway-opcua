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

#ifndef HPP_DDSOPCUA_LOGMSG_HPP_
#define HPP_DDSOPCUA_LOGMSG_HPP_

#include <rti/ddsopcua/log/LogConfig.hpp>

// DDSOPCUA-specific log messages -----------------------------------------

extern const RTILogMessage DDSOPCUA_LOG_OPCUA_ADD_MONITORED_ITEM_FAILED_ss;
extern const RTILogMessage DDSOPCUA_LOG_OPCUA_DELETE_SUBSCRIPTION_FAILED_u;
extern const RTILogMessage DDSOPCUA_LOG_OPCUA_CREATE_SUBSCRIPTION_FAILED_u;
extern const RTILogMessage DDSOPCUA_LOG_OPCUA_CONNECT_SERVER_FAILED_s;
extern const RTILogMessage DDSOPCUA_LOG_XML_LOOKUP_ATTRIBUTE_ss;
extern const RTILogMessage DDSOPCUA_LOG_XML_CONVERT_OBJECT_s;
extern const RTILogMessage DDSOPCUA_LOG_XML_UNSUPPORTED_NODE_IDENTIFIER_s;
extern const RTILogMessage DDSOPCUA_LOG_SEMAPHORE_TAKE_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_THREAD_CREATE_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_OPCUA_RUN_ASYNC_FAILURE_d;
extern const RTILogMessage DDSOPCUA_LOG_XML_READ_ATTRIBUTE_suss;
extern const RTILogMessage DDSOPCUA_LOG_XML_WRITE_ATTRIBUTE_suss;
extern const RTILogMessage DDSOPCUA_LOG_OPCUA_UNKNOWN_ATTRIBUTE_s;
extern const RTILogMessage DDSOPCUA_LOG_OPCUA_UNKNOWN_ATTRIBUTE_u;
extern const RTILogMessage DDSOPCUA_LOG_XML_LOOKUP_SELETOR_s;
extern const RTILogMessage DDSOPCUA_LOG_OPCUA_DISCONNECT_SERVER_FAILED_s;
extern const RTILogMessage DDSOPCUA_LOG_ADD_MONITORED_ITEM_sfud;
extern const RTILogMessage DDSOPCUA_LOG_ADDED_MONITORED_ITEM_ufu;
extern const RTILogMessage DDSOPCUA_LOG_CREATE_SUBSCRIPTION_uduuud;
extern const RTILogMessage DDSOPCUA_LOG_CREATED_SUBSCRIPTION_ufuu;
extern const RTILogMessage DDSOPCUA_LOG_PROCESS_MONITORED_ITEM_uu;
extern const RTILogMessage DDSOPCUA_LOG_CREATE_STREAMREADER_s;
extern const RTILogMessage DDSOPCUA_LOG_XML_LOOKUP_PARENT_s;
extern const RTILogMessage DDSOPCUA_LOG_ANY_s;
extern const RTILogMessage DDSOPCUA_LOG_ANY_ss;
extern const RTILogMessage DDSOPCUA_LOG_CREATION_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_ANY_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_INIT_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_PARSER_ARGUMENT_MISSING_s;
extern const RTILogMessage DDSOPCUA_LOG_PARSER_FAILURE_ss;
extern const RTILogMessage DDSOPCUA_LOG_PARSER_PARSE_FILE_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_PARSER_PARSE_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_PARSER_LOAD_FILE_s;
extern const RTILogMessage DDSOPCUA_LOG_VALIDATOR_VALIDATE_FILE_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_TRANSFORMER_TRANSFORM_FAILURE_ss;
extern const RTILogMessage DDSOPCUA_LOG_VALIDATOR_VALIDATE_ENTITY_FAILURE_s;
extern const RTILogMessage DDSOPCUA_LOG_UNEXPECTED_EXCEPTION;

// Definition of macros that configure LogConfig --------------------------
#define MODULE_DDSOPCUA (0x300000)
#define MODULE_NS DDSOPCUA
#define MODULE_LOG_CONFIG rti::ddsopcua::log::LogConfig::instance()

#endif  // HPP_DDSOPCUA_LOGMSG_HPP_
