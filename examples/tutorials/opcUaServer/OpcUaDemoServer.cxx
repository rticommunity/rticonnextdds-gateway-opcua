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

#include <csignal>
#include <iostream>

#include "OpcUaDemoServer.hpp"
#include "OpcUaDemoNodeSet.hpp"

const char* DDSOPCUA_APPLICATION_NAME = "RTI OPC UA/DDS Gateway";
const char* DDSOPCUA_APPLICATION_URL = "http://www.rti.com";
const UA_UInt16 DDSOPCUA_DEFAULT_PORT = 4840;
UA_Boolean running = true;

void stop_handler(int sign)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

static void print_help(const char* executable_name)
{
    std::cout << "Demo OPC UA Server to test RTI OPC UA/DDS Gateway\n";
    std::cout << "Usage: " << executable_name << " [OPTIONS]*\n";
    std::cout << " -help\t\t\t\tdisplay this help and exit.\n";
    std::cout << " -hostname <name>\t\t\tcustomize hostname for OPC UA "
                 "server.\n";
    std::cout << " -port <NUMBER>\t\t\tchange the port number for OPC UA "
                 "server.\n";
    std::cout << " -updatePeriod <NUMBER (ms)>\tindicates how often must "
                 "the server update the nodes' value.\n";
}

int main(int argc, char** argv)
{
    signal(SIGINT, stop_handler);
    signal(SIGTERM, stop_handler);

    UA_Int16 port = DDSOPCUA_DEFAULT_PORT;
    UA_Int32 update_period = 1000;
    std::string hostname;

    for (int i = 1; i < argc; i++) {
        if (!strncmp(argv[i], "-help", strlen(argv[i]))
            || !strncmp(argv[i], "--help", strlen(argv[i]))
            || !strncmp(argv[i], "-h", strlen(argv[i]))) {
            print_help(argv[0]);
            return 0;
        } else if (
                !strncmp(argv[i], "-port", strlen(argv[i]))
                || !strncmp(argv[i], "--port", strlen(argv[i]))
                || !strncmp(argv[i], "-p", strlen(argv[i]))) {
            if (argv[i + 1] == nullptr) {
                std::cerr << "Error: -port parameter requires an argument\n";
                print_help(argv[0]);
                return 1;
            }
            port = atoi(argv[i + 1]);
            i += 1;
        } else if (
                !strncmp(argv[i], "-hostname", strlen(argv[i]))
                || !strncmp(argv[i], "--hostname", strlen(argv[i]))
                || !strncmp(argv[i], "-n", strlen(argv[i]))) {
            if (argv[i + 1] == nullptr) {
                std::cerr
                        << "Error: -hostname parameter requires an argument\n";
                print_help(argv[0]);
                return 1;
            }
            hostname = argv[i + 1];
            i += 1;
        } else if (
                !strncmp(argv[i], "-updatePeriod", strlen(argv[i]))
                || !strncmp(argv[i], "--updatePeriod", strlen(argv[i]))
                || !strncmp(argv[i], "-u", strlen(argv[i]))) {
            if (argv[i + 1] == nullptr) {
                std::cerr << "Error: -updatePeriod parameter requires an "
                             "argument\n";
                print_help(argv[0]);
                return 1;
            }
            update_period = atoi(argv[i + 1]);
            i += 1;
        } else {
            std::cerr << "Error: unknown parameter " << argv[i] << std::endl;
            print_help(argv[0]);
            return 1;
        }
    }

    using namespace rti::ddsopcua::tutorials;
    OpcUaServer server(port, hostname);

    OpcUaNodeSet nodeset;
    nodeset.add_nodeset(server);

    RepeatedCallbackParam dynamic_scalar_nodes_param = {
        UA_NODEID_NUMERIC(1, 52000),
        0
    };
    server.add_repeated_callback(
            OpcUaNodeSet::update_scalar_node_values,
            (void*) &dynamic_scalar_nodes_param,
            update_period,
            nullptr);

    RepeatedCallbackParam dynamic_array_nodes_param = {
        UA_NODEID_NUMERIC(1, 52100),
        10
    };
    server.add_repeated_callback(
            OpcUaNodeSet::update_array_node_values,
            (void*) &dynamic_array_nodes_param,
            update_period,
            nullptr);

    server.run(running);

    return 0;
}
