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

#include <csignal>
#include <iostream>

#include <dds/core/Duration.hpp>
#include <dds/domain/DomainParticipant.hpp>
#include <rti/util/util.hpp>

#include <rti/ddsopcua/log/LogConfig.hpp>

#include "log/LogMsg.hpp"
#include "service/ApplicationProperty.hpp"

#include "Application.hpp"

static DDS_Boolean terminate_service = false;

static void shutdown_hook_fnc(void*)
{
    terminate_service = DDS_BOOLEAN_TRUE;
    std::cout << "Terminating service" << std::endl;
}

extern "C" void signal_handler(int signal)
{
    switch (signal) {
    case SIGINT:
        std::cout << "Received SIGINT signal" << std::endl;
        shutdown_hook_fnc(nullptr);
        break;
    }
}

int main(int argc, char** argv)
{
    int retcode = 0;
    signal(SIGINT, signal_handler);

    try {
        RTI_RoutingServiceRemoteShutdownHook shutdown_hook = {
            &terminate_service,
            shutdown_hook_fnc
        };

        rti::ddsopcua::service::ApplicationProperty property(
                argc,
                argv,
                shutdown_hook);

        rti::ddsopcua::Application application(property);
        if (application.start()) {
            RTI_RoutingService_execute_heap_snapshot_loop(
                    &property.heap_monitor_property(),
                    -1,
                    &terminate_service);
        }
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
        retcode = 1;
    }

    return retcode;
}
