# (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
# All rights reserved.
#
# RTI grants Licensee a license to use, modify, compile, and create derivative
# works of the Software solely in combination with RTI Connext DDS. Licensee
# may redistribute copies of the Software provided that all such copies are
# subject to this License. The Software is provided "as is", with no warranty
# of any type, including any warranty for fitness for any purpose. RTI is
# under no obligation to maintain or support the Software. RTI shall not be
# liable for any incidental or consequential damages arising out of the use or
# inability to use the Software. For purposes of clarity, nothing in this
# License prevents Licensee from using alternate versions of DDS, provided
# that Licensee may not combine or link such alternate versions of DDS with
# the Software.

cmake_minimum_required(VERSION 3.7)
project(rtigateway-ddsopcua)

option(RTIGATEWAY_BUILD_DOCS "Build documentation" OFF)
option(RTIGATEWAY_BUILD_EXAMPLES "Build examples and tutorials" ON)
option(RTIGATEWAY_BUILD_TESTS "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/rticonnextdds-cmake-utils/cmake/Modules"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/cmake"
)

find_package(
    RTIConnextDDS "6.1.0"
    REQUIRED
    COMPONENTS
        core
        messaging_api
        distributed_logger
        routing_service
)

find_package(PythonInterp REQUIRED)
find_package(EmbedSource REQUIRED)
include(ConnextDdsCodegen)
include(ConnextDdsGenerateFiles)

connextdds_copy_files(
    INPUT_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/schema/rti_dds_opcua_service.xsd"
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/resources/schema"
)

connextdds_copy_files(
    INPUT_FILES
        # DDS
        "${CONNEXTDDS_DIR}/resource/schema/definitions/rti_dds_qos_profiles_definitions.xsd"
        "${CONNEXTDDS_DIR}/resource/schema/definitions/rti_dds_profiles_definitions.xsd"
        "${CONNEXTDDS_DIR}/resource/schema/definitions/rti_dds_topic_types_definitions.xsd"

        # Distributed Logger
        "${CONNEXTDDS_DIR}/resource/schema/definitions/rti_dist_logger_definitions.xsd"

        # Routing Service
        "${CONNEXTDDS_DIR}/resource/schema/definitions/rti_service_common_definitions.xsd"
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/resources/schema/definitions"
)

connextdds_copy_files(
    INPUT_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/xml/RTI_DDS_OPCUA_SERVICE.xml"
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/xml/OpcUaBuiltinDataTypes.xml"
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/resources/xml"
)

connextdds_rtiddsgen_run(
    IDL_FILE
        "${CMAKE_CURRENT_BINARY_DIR}/resources/xml/OpcUaBuiltinDataTypes.xml"
    OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/src/conversion/"
    LANG C++11
    UNBOUNDED
)

connextdds_embed_source_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/schema/rti_dds_opcua_service.xsd"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config/rti_dds_opcua_service_xsd.hpp"
    DDSOPCUA_XSD
)

connextdds_embed_source_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/xslt/rti_ddsopcua_to_router.xslt"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config/rti_ddsopcua_to_router_xslt.hpp"
    DDSOPCUA_XSLT
)

connextdds_embed_source_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/xml/OpcUaBuiltinDataTypes.xml"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config/OpcUaBuiltinDataTypes_xml.hpp"
    OPCUA_BUILTIN_TYPES_XML
)

set(script_extension "")
if(WIN32)
    set(script_extension ".bat")
endif()

set(LAUNCH_EXECUTABLE_NAME "rtiddsopcuagateway")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/templates/launch_script${script_extension}.in"
    "${CMAKE_CURRENT_BINARY_DIR}/scripts/${LAUNCH_EXECUTABLE_NAME}${script_extension}"
)

# Build open62541

set(UA_NAMESPACE_ZERO "FULL")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/open62541"
    EXCLUDE_FROM_ALL
)

set_target_properties(open62541 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} output_config)
    set_target_properties(open62541 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        PDB_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        COMPILE_PDB_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endforeach()

# Build Gateway

add_library(rtiddsopcuagateway-lib
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/detail/DdsOpcUaGatewayImpl.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/DdsOpcUaGatewayException.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/DdsOpcUaGatewayProperty.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/requester/detail/DdsRequesterOperations.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/requester/detail/OpcUaTypes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/requester/DdsRequester.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/DdsOpcUaGateway.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/rti/ddsopcua/log/LogConfig.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/RtiXmlUtilsDecl.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/XmlEntities.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/XmlSupport.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/XmlTransformationParams.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/XmlEntities.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/XmlSupport.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/XmlTransformationParams.cxx"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config/rti_dds_opcua_service_xsd.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config/rti_ddsopcua_to_router_xslt.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config/OpcUaBuiltinDataTypes_xml.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/conversion/DdsDynamicDataUtils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/conversion/OpcUa2DdsDynamicDataConversions.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/conversion/DdsDynamicData2OpcUaConversions.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/conversion/DdsDynamicDataUtils.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/conversion/OpcUa2DdsDynamicDataConversions.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/conversion/DdsDynamicData2OpcUaConversions.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log/LogMsg.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log/LogMsg.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log/LoggerImpl.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/OpcUaSdkClient.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/OpcUaSdkClientProperty.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/OpcUaSdkClientMonitoredItemProperty.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/OpcUaSdkClientSubscriptionProperty.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/OpcUaSdkDataTypes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/OpcUaSdkClient.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/OpcUaSdkDataTypes.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/DdsOpcUaAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/DdsOpcUaAdapter.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/DdsOpcUaAdapterProperty.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/DdsOpcUaAdapterProperty.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaAttributeServiceStreamReader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaAttributeServiceStreamReader.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaAttributeServiceStreamWriter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaAttributeServiceStreamWriter.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaConnection.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaConnection.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaPublishRequestThread.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaSubscriptionStreamReader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaSubscriptionStreamReader.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaAttributeServiceStreamWriter.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/adapters/OpcUaAttributeServiceStreamWriter.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/processors/OpcUaAttributeServiceProcessor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/processors/OpcUaAttributeServiceProcessor.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/processors/OpcUaPublicationProcessor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/processors/OpcUaPublicationProcessor.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/requester/DdsRequester.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/requester/DdsRequesterOperations.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/requester/OpcUaTypes.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/ApplicationProperty.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/ParameterManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/Service.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/Utils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/RtiRoutingServiceImplDecl.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/ApplicationProperty.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/ParameterManager.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/DdsOpcUaService.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/Service.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/service/ServiceProperty.cxx"
    ${OpcUaBuiltinDataTypes_CXX11_HEADERS}
    ${OpcUaBuiltinDataTypes_CXX11_SOURCES}
)

target_include_directories(rtiddsopcuagateway-lib
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}/src"
)
target_link_libraries(rtiddsopcuagateway-lib
    RTIConnextDDS::cpp2_api
    RTIConnextDDS::rtixml2
    RTIConnextDDS::messaging_c_api
    RTIConnextDDS::messaging_cpp2_api
    RTIConnextDDS::metp
    RTIConnextDDS::distributed_logger_c
    RTIConnextDDS::routing_service_infrastructure
    RTIConnextDDS::routing_service_cpp2
    open62541::open62541
)

set_target_properties(rtiddsopcuagateway-lib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    OUTPUT_NAME "rtiddsopcuagateway"
)

if(WIN32)
    set_target_properties(rtiddsopcuagateway-lib PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
    )
endif()

# Multiconfiguration projects have different output directories
# for each CMAKE_CONFIGURATION_TYPES
foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} output_config)
    set_target_properties(rtiddsopcuagateway-lib PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endforeach()


# Build Executable
add_executable(rtiddsopcuagateway-app
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serviceApp/Application.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serviceApp/Application.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/serviceApp/Main.cxx"
)
target_include_directories(rtiddsopcuagateway-app
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}/src"
)

target_link_libraries(rtiddsopcuagateway-app
    rtiddsopcuagateway-lib
)

set_target_properties(rtiddsopcuagateway-app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    OUTPUT_NAME "rtiddsopcuagatewayapp"
)

# Multiconfiguration projects have different output directories
# for each CMAKE_CONFIGURATION_TYPES
foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} output_config)
    set_target_properties(rtiddsopcuagateway-app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endforeach()

install(TARGETS rtiddsopcuagateway-app
    RUNTIME DESTINATION resource/app/bin/${CONNEXTDDS_ARCH}
)

install(TARGETS rtiddsopcuagateway-lib
    LIBRARY DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
    RUNTIME DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
    ARCHIVE DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
)

install(TARGETS open62541
    LIBRARY DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
    RUNTIME DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
    ARCHIVE DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
    DESTINATION "include"
    USE_SOURCE_PERMISSIONS
)

install(DIRECTORY "${CMAKE_BINARY_DIR}/scripts/"
    DESTINATION "bin"
    FILE_PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
)

install(DIRECTORY "${CMAKE_BINARY_DIR}/resources/schema/"
   DESTINATION resource/schema
   USE_SOURCE_PERMISSIONS
)

# Configuration files
install(DIRECTORY "${CMAKE_BINARY_DIR}/resources/xml/"
   DESTINATION resource/xml
   USE_SOURCE_PERMISSIONS
)

if (BUILD_SHARED_LIBS)
    install(FILES
        ${CONNEXTDDS_CPP_API_LIBRARIES_RELEASE_SHARED}
        ${CONNEXTDDS_CPP2_API_LIBRARIES_RELEASE_SHARED}
        ${ROUTING_SERVICE_API_LIBRARIES_RELEASE_SHARED}
        ${ROUTING_SERVICE_INFRASTRUCTURE_LIBRARIES_RELEASE_SHARED}
        ${MESSAGING_CPP2_API_LIBRARIES_RELEASE_SHARED}
        ${METP_LIBRARIES_RELEASE_SHARED}
        ${RTIXML2_LIBRARIES_RELEASE_SHARED}
        ${RTIAPPUTILS_C_LIBRARIES_RELEASE_SHARED}
        CONFIGURATIONS Release
        DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
    )

    install(FILES
        ${CONNEXTDDS_CPP_API_LIBRARIES_DEBUG_SHARED}
        ${CONNEXTDDS_CPP2_API_LIBRARIES_DEBUG_SHARED}
        ${ROUTING_SERVICE_API_LIBRARIES_DEBUG_SHARED}
        ${ROUTING_SERVICE_INFRASTRUCTURE_LIBRARIES_DEBUG_SHARED}
        ${MESSAGING_CPP2_API_LIBRARIES_DEBUG_SHARED}
        ${METP_LIBRARIES_DEBUG_SHARED}
        ${RTIXML2_LIBRARIES_DEBUG_SHARED}
        ${RTIAPPUTILS_C_LIBRARIES_DEBUG_SHARED}
        CONFIGURATIONS Debug
        DESTINATION resource/app/lib/${CONNEXTDDS_ARCH}
    )
endif()

# Common Script
if (WIN32)
    install(PROGRAMS "${CONNEXTDDS_DIR}/resource/scripts/rticommon.bat"
        DESTINATION resource/scripts
    )
else()
    install(PROGRAMS "${CONNEXTDDS_DIR}/resource/scripts/rticommon.sh"
        DESTINATION resource/scripts
    )
endif()

# Build tests
if (RTIGATEWAY_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest"
        EXCLUDE_FROM_ALL
    )
    set_target_properties(gtest PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
    foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${output_config} output_config)
        set_target_properties(gtest PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
            PDB_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
            COMPILE_PDB_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        )
    endforeach()

    connextdds_copy_files(
        INPUT_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/resources/xml/IntegrationTestTypes.xml"
        OUTPUT_DIR
            "${CMAKE_CURRENT_BINARY_DIR}/resources/xml"
    )

    connextdds_rtiddsgen_run(
        IDL_FILE
            "${CMAKE_CURRENT_BINARY_DIR}/resources/xml/IntegrationTestTypes.xml"
        OUTPUT_DIRECTORY
            "${CMAKE_CURRENT_BINARY_DIR}/src/test/"
        LANG C++11
        UNBOUNDED
    )

    set(rtiddsopcua_test_sources
        "${CMAKE_CURRENT_SOURCE_DIR}/src/conversion/test/ConversionTests.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorials/opcUaServer/OpcUaDemoNodeSet.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/tutorials/opcUaServer/OpcUaDemoServer.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/serviceApp/Application.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/serviceApp/Application.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/serviceApp/test/ServiceAppTest.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/test/OpcUaDataTypesTester.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/opcUaSdk/test/OpcUaSdkTester.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/test/OpcUaTutorialServer.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/test/IntegrationTest.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/test/Main.cxx"
    )
    add_executable(rtiddsopcuagateway-test
        ${rtiddsopcua_test_sources}
        ${IntegrationTestTypes_CXX11_HEADERS}
        ${IntegrationTestTypes_CXX11_SOURCES}
    )
    target_include_directories(rtiddsopcuagateway-test
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${CMAKE_CURRENT_SOURCE_DIR}/examples"
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
            "${CMAKE_CURRENT_BINARY_DIR}/src"
            "${CMAKE_CURRENT_BINARY_DIR}/src/conversion"
            "${CMAKE_CURRENT_BINARY_DIR}/src/test"
    )
    target_link_libraries(rtiddsopcuagateway-test
        gtest
        rtiddsopcuagateway-lib
    )
    set_target_properties(rtiddsopcuagateway-test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        OUTPUT_NAME "rtiddsopcuagateway-tester"
    )

    # Multiconfiguration projects have different output directories
    # for each CMAKE_CONFIGURATION_TYPES
    foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${output_config} output_config)
        set_target_properties(rtiddsopcuagateway-test PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
        )
    endforeach()

    set(env_var "LD_LIBRARY_PATH")
    set(env_sep ":")
    if (WIN32)
        set(env_var "PATH")
        set(env_sep "\;")
    elseif(APPLE)
        set(env_var "DYLD_LIBRARY_PATH")
    endif()
    gtest_add_tests(TARGET rtiddsopcuagateway-test
        SOURCES ${rtiddsopcua_test_sources}
        TEST_LIST ctest_list
    )
    set_tests_properties(${ctest_list} PROPERTIES
        ENVIRONMENT "${env_var}=$<SHELL_PATH:${CMAKE_CURRENT_BINARY_DIR}/bin>${env_sep}$<SHELL_PATH:${CONNEXTDDS_DIR}/lib/${CONNEXTDDS_ARCH}>"
    )

    if (RTIGATEWAY_ENABLE_CODE_COVERAGE)
        include(CodeCoverage)
        append_coverage_compiler_flags()
        setup_target_for_coverage_lcov(
            NAME rtiddsopcuagateway-code-coverage
            EXECUTABLE rtiddsopcuagateway-test
            BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            LCOV_ARGS "--no-external"
            EXCLUDE
                "${CMAKE_CURRENT_SOURCE_DIR}/third_party/*"
                "${CMAKE_CURRENT_SOURCE_DIR}/examples/*"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/serviceApp/Main.cxx"
                "${CMAKE_CURRENT_BINARY_DIR}/*"
        )
    endif()
endif()

# Build examples

if (RTIGATEWAY_BUILD_EXAMPLES)
    # Tutorial examples
    add_subdirectory(examples/tutorials)
endif()
