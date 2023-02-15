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

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

#include <rti/ddsopcua/requester/DdsRequester.hpp>

#include "RequesterProperties.hpp"
#include "CommandlineParser.hpp"

using namespace rti::ddsopcua;

int main(int argc, char** argv)
{
    utils::setup_signal_handlers();

    int retcode = EXIT_SUCCESS;

    requester::RequesterProperties properties;
    if (utils::parse_arguments(properties, argc, argv)
            != utils::ParseRetcode::PARSE_RETCODE_OK) {
        return EXIT_FAILURE;
    }

    requester::Requester::initialize_globals();

    try {
        // Create Requester
        requester::Requester requester(properties.dds_domain_id);

        // Wait until it discovers the Gateway
        if (!requester.wait_for_matching_gateway(10, utils::exit_condition)) {
            throw std::runtime_error("Error: Gateway not found!");
        }

        for (uint32_t i = 0; i < properties.iterations; i++) {
            if (properties.service_operation
                == requester::ServiceOperation::SERVICE_OPERATION_READ) {
                requester.read_request(
                        properties.variant,
                        properties.server_id,
                        properties.node_id);
            } else if (
                    properties.service_operation
                    == requester::ServiceOperation::SERVICE_OPERATION_WRITE) {
                requester.write_request(
                        properties.server_id,
                        properties.node_id,
                        properties.variant);
            } else {
                throw std::runtime_error("Error: Unsupported operation");
            }
            std::this_thread::sleep_for(
                    std::chrono::milliseconds(properties.period_msec));
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        retcode = EXIT_FAILURE;
    }

    requester::Requester::finalize_globals();

    return retcode;
}
