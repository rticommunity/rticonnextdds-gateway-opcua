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

#ifndef RTI_OPCUA_ADAPTER_DDSOPCUACONNECTIONTHREAD_HPP_
#define RTI_OPCUA_ADAPTER_DDSOPCUACONNECTIONTHREAD_HPP_

#include <dds/core/Duration.hpp>
#include <rti/util/util.hpp>

#include "opcUaSdk/OpcUaSdkClient.hpp"
#include "service/Utils.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

class AsyncClientThread : public utils::Thread {
public:
    /**
     * The constructor of the SendPublishRequestThread class takes the
     * both the thread_name and a pointer to the UaClient as a parameter,
     * and sets the exit condition to false so that it can start running as
     * soon as the start() method is called.
     * @param thread_name Name of the thread that will be created.
     * @param opcua_client Pointer to the UaClient object
     * @param sleep_period Time to wait between send publish requests
     */
    AsyncClientThread(
            opcua::sdk::client::Client& opcua_client,
            rti::ddsopcua::utils::ServiceShutdownHook& service_shutdown_hook,
            const std::string& thread_name)
            : Thread(thread_name),
              opcua_client_(opcua_client),
              service_shutdown_hook_(service_shutdown_hook),
              exit_condition_(false),
              timeout_(0)
    {
    }

    ~AsyncClientThread()
    {
        GATEWAYLog_local(
                &DDSOPCUA_LOG_ANY_s,
                "Deleting asynchronous client thread");
    }
    /**
     * This method sends periodic SendPublishRequests. To avoid spinning the
     * thread sleeps every sleep_time. Note that the infinite loop has an exit
     * condition that is called by the destructor of the class.
     */
    void run()
    {
        uint32_t retcode = 0;
        while (!exit_condition_) {
            retcode = opcua_client_.run_iterate(timeout_);
            if (retcode != 0) {
                exit_condition_ = true;
                service_shutdown_hook_.shutdown_service();
            }
        }
    }

    /**
     * This method extends the Thread class setting an exit condition and
     * calling join, to make sure that when the destructor is called all
     * is in order.
     */
    void stop()
    {
        exit_condition_ = true;
        GATEWAYLog_local(
                &DDSOPCUA_LOG_ANY_s,
                "Stopping asynchronous client thread");
        join();
        GATEWAYLog_local(
                &DDSOPCUA_LOG_ANY_s,
                "Asynchronous client thread stopped");
    }

    /**
     * This parameter updates the value of sleep period to allow sending
     * periodic SendPublishRequests faster or slower.
     * @param sleep_period New value of sleep_period
     */
    void reset(opcua::sdk::client::Client& opcua_client)
    {
        opcua_client_ = opcua_client;
    }

    void timeout(uint16_t value)
    {
        timeout_ = value;
    }

private:
    opcua::sdk::client::Client& opcua_client_;
    rti::ddsopcua::utils::ServiceShutdownHook& service_shutdown_hook_;
    bool exit_condition_;
    uint16_t timeout_;
};

}}}  // namespace rti::ddsopcua::adapters

#endif  // RTI_OPCUA_ADAPTER_DDSOPCUACONNECTIONTHREAD_HPP_
