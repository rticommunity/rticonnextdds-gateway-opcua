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

#ifndef RTI_DDSOPCUA_GENERIC_REQUESTER_COMMANDLINE_PARSER_HPP_
#define RTI_DDSOPCUA_GENERIC_REQUESTER_COMMANDLINE_PARSER_HPP_

#include <csignal>

#include "Conversions.hpp"

namespace rti { namespace ddsopcua { namespace utils {

bool exit_condition = false;

void stop_handler(int)
{
    exit_condition = true;
}

void setup_signal_handlers()
{
    signal(SIGINT, stop_handler);
    signal(SIGTERM, stop_handler);
}

enum ParseRetcode { PARSE_RETCODE_OK, PARSE_RETCODE_ERROR, PARSE_RETCODE_EXIT };

void print_help()
{
    std::cout << "usage:\trtiddsopcuarequester [--help]" << std::endl
              << "\t\t\t     <command> [<args>]" << std::endl
              << std::endl;
    std::cout << "These are the supported commands:\n\n";

    // Read command
    std::cout << " read\tReads the value of a Variable Node in the address"
                 "space of an OPC UA Server.\n\n";

    std::cout << "\trtiddsopcuarequester read "
                 "--server-id <server_id>\n"
              << "\t\t\t\t  --node-id <namespace_index>:<identifier>\n"
              << "\t\t\t\t  [--iterations <number_of_iterations>]\n"
              << "\t\t\t\t  [--period <milliseconds_between_reads>]\n"
              << "\t\t\t\t  [--domain-id <dds_domain_id>]\n";

    std::cout << std::endl;

    std::cout << " write\tUpdates the value of a Variable Node in the address"
                 "space of an OPC UA Server.\n\n";

    std::cout << "\trtiddsopcuarequester write "
                 "--server-id <server_id>\n"
              << "\t\t\t\t   --node-id <namespace_index>:<identifier>\n"
              << "\t\t\t\t   --type \"<value_type>\"\n"
              << "\t\t\t\t   --value \"<value>\"\n"
              << "\t\t\t\t   [--iterations <number_of_iterations>]\n"
              << "\t\t\t\t   [--period <milliseconds_between_writes>]\n"
              << "\t\t\t\t   [--domain-id <dds_domain_id>]\n";
}

ParseRetcode parse_arguments(
        requester::RequesterProperties& properties,
        int argc,
        char** argv)
{
    if (argc < 2) {
        std::cerr << "Error: " << argv[0] << " requires an argument.\n";
        print_help();
        return PARSE_RETCODE_ERROR;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_help();
        return PARSE_RETCODE_EXIT;
    }

    if (strcmp(argv[1], "read") == 0) {
        properties.service_operation =
                requester::ServiceOperation::SERVICE_OPERATION_READ;
    } else if (strcmp(argv[1], "write") == 0) {
        properties.service_operation =
                requester::ServiceOperation::SERVICE_OPERATION_WRITE;
    } else {
        std::cerr << "Error: invalid argument " << argv[1] << std::endl;
        return PARSE_RETCODE_ERROR;
    }

    bool node_id_set = false;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--node-id") == 0) {
            if (i + 1 > argc) {
                std::cerr << "Error: --node-id requires an argument.\n";
                return PARSE_RETCODE_ERROR;
            }
            if (!conversions::string_to_node_id(
                        properties.node_id,
                        argv[++i])) {
                return PARSE_RETCODE_ERROR;
            }
            node_id_set = true;
        } else if (strcmp(argv[i], "--server-id") == 0) {
            if (i + 1 > argc) {
                std::cerr << "Error: --server-id requires an argument.\n";
                return PARSE_RETCODE_ERROR;
            }
            properties.server_id = argv[++i];
        } else if (strcmp(argv[i], "--value") == 0) {
            if (i + 1 > argc) {
                std::cerr << "Error: --value requires an argument.\n";
                return PARSE_RETCODE_ERROR;
            }
            properties.value_str = argv[++i];
        } else if (strcmp(argv[i], "--type") == 0) {
            if (i + 1 > argc) {
                std::cerr << "Error: --type requires an argument.\n";
                return PARSE_RETCODE_ERROR;
            }
            properties.value_type =
                    conversions::string_to_opc_ua_type(argv[++i]);
        } else if (strcmp(argv[i], "--domain-id") == 0) {
            if (i + 1 > argc) {
                std::cerr << "Error: --domain-id requires an argument.\n";
                return PARSE_RETCODE_ERROR;
            }
            properties.dds_domain_id = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--iterations") == 0) {
            if (i + 1 > argc) {
                std::cerr << "Error: --iterations requires an argument.\n";
                return PARSE_RETCODE_ERROR;
            }
            properties.iterations = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--period") == 0) {
            if (i + 1 > argc) {
                std::cerr << "Error: --period requires an argument.\n";
                return PARSE_RETCODE_ERROR;
            }
            properties.period_msec = atoi(argv[++i]);
        } else if (
                strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help();
            return PARSE_RETCODE_EXIT;
        }
    }
    if (!node_id_set) {
        std::cerr << "Error: missing --node-id argument." << std::endl;
        return PARSE_RETCODE_ERROR;
    }
    if (properties.server_id.empty()) {
        std::cerr << "Error: missing --server-id argument." << std::endl;
        return PARSE_RETCODE_ERROR;
    }
    if (properties.service_operation
        == requester::ServiceOperation::SERVICE_OPERATION_WRITE) {
        if (properties.value_str.empty()) {
            std::cerr << "Error: missing --value argument." << std::endl;
            return PARSE_RETCODE_ERROR;
        }
        if (properties.value_type
            == rti::opcua::types::BuiltinTypeKind::DIAGNOSTICINFO_TYPE) {
            std::cerr << "Error: missing --type argument." << std::endl;
            return PARSE_RETCODE_ERROR;
        }

        // received value into a Variant
        if (!conversions::string_to_variant(
                    properties.variant,
                    properties.value_type,
                    properties.value_str)) {
            return PARSE_RETCODE_ERROR;
        }
    }

    return PARSE_RETCODE_OK;
}

}}}  // namespace rti::ddsopcua::utils

#endif  // RTI_DDSOPCUA_GENERIC_REQUESTER_COMMANDLINE_PARSER_HPP_
