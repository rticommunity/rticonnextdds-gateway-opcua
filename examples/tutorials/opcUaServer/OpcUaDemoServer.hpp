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

#ifndef RTI_DDSOPCUA_TUTORIALS_OPCUA_SERVER_HPP_
#define RTI_DDSOPCUA_TUTORIALS_OPCUA_SERVER_HPP_

#include <string>

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

namespace rti { namespace ddsopcua { namespace tutorials {

class OpcUaServer {
public:
    OpcUaServer(UA_UInt16 port, const std::string hostname = "")
            : server_(nullptr)
    {
        // UA_ServerConfig config;
        // memset(&config, 0, sizeof(UA_ServerConfig));
        // UA_ServerConfig_setMinimal(&config, port, nullptr);
        // config_->applicationDescription.applicationName =
        // UA_LOCALIZEDTEXT_ALLOC(
        //         const_cast<char*>("en-US"),
        //         const_cast<char*>(DDSOPCUA_APPLICATION_NAME));
        // config_->applicationDescription.applicationUri = UA_STRING_ALLOC(
        //         const_cast<char*>(DDSOPCUA_APPLICATION_URL));
        // config_->buildInfo.productName = UA_STRING_ALLOC(
        //         const_cast<char*>(DDSOPCUA_APPLICATION_NAME));
        // config_->buildInfo.productUri = UA_STRING_ALLOC(
        //         const_cast<char*>(DDSOPCUA_APPLICATION_URL));
        // server_ = UA_Server_newWithConfig(&config);
        server_ = UA_Server_new();
        UA_ServerConfig* config = UA_Server_getConfig(server_);
        UA_ServerConfig_setMinimal(config, port, nullptr);

        if (hostname != "") {
            config->customHostname =
                    UA_STRING(const_cast<char*>(hostname.c_str()));
        }
    }

    ~OpcUaServer()
    {
        UA_Server_delete(server_);
        // UA_LocalizedText_deleteMembers(
        //         &config_->applicationDescription.applicationName);
        // UA_String_delete(&config_->applicationDescription.applicationUri);
        // UA_String_delete(&config_->buildInfo.productName);
        // UA_String_delete(&config_->buildInfo.productUri);
        server_ = nullptr;
    }

    bool run(bool& running)
    {
        UA_StatusCode retval = UA_Server_run(server_, &running);
        if (retval != UA_STATUSCODE_GOOD) {
            return false;
        }

        return true;
    }

    uint16_t run_iterate()
    {
        uint16_t timeout = UA_Server_run_iterate(server_, false);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = timeout * 1000;
        select(0, NULL, NULL, NULL, &tv);
        return timeout;
    }

    bool add_repeated_callback(
            UA_ServerCallback callback,
            void* data,
            UA_UInt32 interval,
            UA_UInt64* id)
    {
        UA_StatusCode retval = UA_Server_addRepeatedCallback(
                server_,
                callback,
                data,
                interval,
                id);
        if (retval != UA_STATUSCODE_GOOD) {
            return false;
        }

        return true;
    }

    UA_Server* native_server() const
    {
        return server_;
    }

private:
    UA_Server* server_;
};

}}}  // namespace rti::ddsopcua::tutorials

#endif  // RTI_DDSOPCUA_TUTORIALS_OPCUA_SERVER_HPP_
