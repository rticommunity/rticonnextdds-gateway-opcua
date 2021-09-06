#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# RTI Recording Service Service Links Definitions for User's Manual
#
# This file contains a variable that represents the product version
#

RTI_VERSION = '6.1.0'
RTI_VERSION_SHORT = '6.1.0'

LINK_XML_SPEC = 'https://www.w3.org/TR/2008/REC-xml-20081126/'

LINK_CONNEXT_DOC_PREFIX = 'https://community.rti.com/static/' \
    'documentation/connext-dds/%s/doc/manuals' \
    % RTI_VERSION_SHORT

LINK_CONNEXT_API_PREFIX = 'https://community.rti.com/static/' \
    'documentation/connext-dds/%s/doc/api' \
    % RTI_VERSION_SHORT

LINK_CONNEXT_USER_MAN_PREFIX = '%s/connext_dds/html_files/' \
    'RTI_ConnextDDS_CoreLibraries_UsersManual/index.htm#UsersManual' \
    % LINK_CONNEXT_DOC_PREFIX

LINK_CONNEXT_USER_XML_CONFIG = '%s/XMLConfiguration.htm' \
    % LINK_CONNEXT_USER_MAN_PREFIX

LINK_CONNEXT_USER_CFT_CONFIG = '%s/ContentFilteredTopics.htm' \
    % LINK_CONNEXT_USER_MAN_PREFIX

LINK_CONNEXT_USER_MAN_DISTLOG = '%sEnablingDistributedLoggerInRTIServices.htm' \
    % LINK_CONNEXT_USER_MAN_PREFIX

LINK_CONNEXT_USER_MAN_TYPES_XML = '%s/Creating_User_Data_Types_with_Extensible.htm' \
    % LINK_CONNEXT_USER_MAN_PREFIX

LINK_CONNEXT_USER_MAN_LARGEDATA= '%s' \
    '/SendingLargeData.htm' \
    % LINK_CONNEXT_USER_MAN_PREFIX

LINK_ROUTER_MAN = '%s/routing_service/' \
    % LINK_CONNEXT_DOC_PREFIX

LINK_ROUTER_MAN_CONFIG = '%s/routing_service/configuration.html' \
    % LINK_CONNEXT_DOC_PREFIX

LINK_ROUTER_MAN_CONFIG_TRANSFORMATION = '%s#data-transformation' \
    % LINK_ROUTER_MAN_CONFIG

LINK_ROUTER_MAN_TUTORIALS = '%s/routing_service/tutorials.html' \
    % LINK_CONNEXT_DOC_PREFIX

LINK_SQLITE_DOWNLOAD = 'https://sqlite.org/download.html'

LINK_C_API_DOCS = '%s/recording_service/api_c/index.html' \
    % LINK_CONNEXT_API_PREFIX

LINK_CPP_API_DOCS = '%s/recording_service/api_cpp/index.html' \
    % LINK_CONNEXT_API_PREFIX

LINK_CPP_API_SERVICE_DOCS = '%s/recording_service/api_cpp/group__RTI__RecordingServiceLibModule.html' \
    % LINK_CONNEXT_API_PREFIX

LINK_COMMUNITY_EXAMPLES = \
    'https://github.com/rticommunity/rticonnextdds-examples/tree/master/examples/'

LINK_COMMUNITY_EXAMPLES_RECORDING_C_STORAGE = '%s' \
    'recording_service/pluggable_storage/c' \
     % LINK_COMMUNITY_EXAMPLES

LINK_COMMUNITY_EXAMPLES_RECORDING_CPP_STORAGE = '%s' \
    'recording_service/pluggable_storage/cpp' \
     % LINK_COMMUNITY_EXAMPLES

LINK_COMMUNITY_EXAMPLES_RECORDING_SERVICE_ADMIN = '%s' \
    'recording_service/service_admin/cpp' \
     % LINK_COMMUNITY_EXAMPLES

LINK_COMMUNITY_EXAMPLES_RECORDING_SERVICE_AS_LIB= '%s' \
    'recording_service/service_as_lib/cpp' \
     % LINK_COMMUNITY_EXAMPLES
