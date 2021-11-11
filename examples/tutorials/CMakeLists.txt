# OPC UA Demo Server

add_executable(tutorial-opc-ua-server
    "${CMAKE_CURRENT_SOURCE_DIR}/opcUaServer/OpcUaDemoNodeSet.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/opcUaServer/OpcUaDemoServer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/opcUaServer/OpcUaDemoServer.cxx"
)
target_link_libraries(tutorial-opc-ua-server
    open62541::open62541
)
set_target_properties(tutorial-opc-ua-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} output_config)
    set_target_properties(tutorial-opc-ua-server PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endforeach()

install(TARGETS tutorial-opc-ua-server
    RUNTIME DESTINATION resource/app/bin/${CONNEXTDDS_ARCH}
)

set(LAUNCH_EXECUTABLE_NAME "tutorial-opc-ua-server")
configure_file(
    "${CMAKE_SOURCE_DIR}/resources/templates/launch_script${RTIGATEWAY_SCRIPT_EXTENSION}.in"
    "${CMAKE_BINARY_DIR}/scripts/${LAUNCH_EXECUTABLE_NAME}${RTIGATEWAY_SCRIPT_EXTENSION}"
)

# DDS Demo Publisher

add_executable(tutorial-dds-publisher
    "${CMAKE_CURRENT_SOURCE_DIR}/ddsPublisher/DdsDemoPublisher.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ddsPublisher/DdsDemoPublisher.cxx"
)
target_link_libraries(tutorial-dds-publisher
    RTIConnextDDS::cpp2_api
)
set_target_properties(tutorial-dds-publisher
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"

)
foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} output_config)
    set_target_properties(tutorial-dds-publisher PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endforeach()

install(TARGETS tutorial-dds-publisher
    RUNTIME DESTINATION resource/app/bin/${CONNEXTDDS_ARCH}
)

set(LAUNCH_EXECUTABLE_NAME "tutorial-dds-publisher")
configure_file(
    "${CMAKE_SOURCE_DIR}/resources/templates/launch_script${RTIGATEWAY_SCRIPT_EXTENSION}.in"
    "${CMAKE_BINARY_DIR}/scripts/${LAUNCH_EXECUTABLE_NAME}${RTIGATEWAY_SCRIPT_EXTENSION}"
)

# DDS Demo Requester

connextdds_rtiddsgen_run(
    IDL_FILE
        "${CMAKE_BINARY_DIR}/resources/xml/OpcUaBuiltinDataTypes.xml"
    OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/src/conversion/"
    LANG C++11
    UNBOUNDED
    EXTRA_ARGS "-disableXSDValidation"
)

 add_executable(tutorial-dds-requester
    "${CMAKE_CURRENT_SOURCE_DIR}/ddsRequester/CommandlineParser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ddsRequester/Conversions.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ddsRequester/Main.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/ddsRequester/RequesterProperties.hpp"
    ${OpcUaBuiltinDataTypes_CXX11_HEADERS}
    ${OpcUaBuiltinDataTypes_CXX11_SOURCES}
)
target_include_directories(tutorial-dds-requester PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_BINARY_DIR}/src"
)
target_link_libraries(tutorial-dds-requester
    rtiddsopcuagateway-lib
)
set_target_properties(tutorial-dds-requester PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
foreach (output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} output_config)
    set_target_properties(tutorial-dds-requester PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${output_config} "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )
endforeach()

install(TARGETS tutorial-dds-requester
    RUNTIME DESTINATION resource/app/bin/${CONNEXTDDS_ARCH}
)

set(LAUNCH_EXECUTABLE_NAME "tutorial-dds-requester")
configure_file(
    "${CMAKE_SOURCE_DIR}/resources/templates/launch_script${RTIGATEWAY_SCRIPT_EXTENSION}.in"
    "${CMAKE_BINARY_DIR}/scripts/${LAUNCH_EXECUTABLE_NAME}${RTIGATEWAY_SCRIPT_EXTENSION}"
)
