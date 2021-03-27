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

#ifndef RTI_DDSOPCUA_TEST_TUTORIALSERVER_HPP_
#define RTI_DDSOPCUA_TEST_TUTORIALSERVER_HPP_

#include <thread>

#include "tutorials/opcUaServer/OpcUaDemoNodeSet.hpp"
#include "tutorials/opcUaServer/OpcUaDemoServer.hpp"

namespace rti { namespace ddsopcua { namespace test {

class OpcUaTutorialServer {
public:
    OpcUaTutorialServer(uint16_t port = 4840) : running_(false), server_(port)
    {
        nodeset_.add_nodeset(server_);
    }

    ~OpcUaTutorialServer()
    {
        if (!running_) {
            return;
        }
        running_ = false;
        server_thread_.join();
    }

    void update_nodes()
    {
        rti::ddsopcua::tutorials::RepeatedCallbackParam scalar_nodes = {
            UA_NODEID_NUMERIC(1, 52000),
            0
        };
        rti::ddsopcua::tutorials::RepeatedCallbackParam array_nodes = {
            UA_NODEID_NUMERIC(1, 52100),
            10
        };

        nodeset_.update_scalar_node_values(
                server_.native_server(),
                static_cast<void*>(&scalar_nodes));
        nodeset_.update_array_node_values(
                server_.native_server(),
                static_cast<void*>(&array_nodes));
    }

    void start()
    {
        running_ = true;
        server_thread_ = std::thread(
                run_opc_ua_server,
                std::ref(server_),
                std::ref(running_));
    }

    void stop()
    {
        running_ = false;
        server_thread_.join();
    }

private:
    static void run_opc_ua_server(
            rti::ddsopcua::tutorials::OpcUaServer& server,
            bool& running)
    {
        server.run(running);
    }


private:
    bool running_;
    rti::ddsopcua::tutorials::OpcUaServer server_;
    rti::ddsopcua::tutorials::OpcUaNodeSet nodeset_;
    std::thread server_thread_;
};

}}}  // namespace rti::ddsopcua::test

#endif  // RTI_DDSOPCUA_TEST_TUTORIALSERVER_HPP_