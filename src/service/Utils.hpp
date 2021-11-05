/*
 * (c) 2019-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#ifndef SERVICE_UTILS_HPP_
#define SERVICE_UTILS_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>

#include <dds/core/types.hpp>
#include <osapi/osapi_thread.h>
#include <osapi/osapi_utility.h>
#include <rti/core/Semaphore.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include "log/LogMsg.hpp"


#define STATIC_CONST_STRING_DEFINITION(classname, name, value) \
    const std::string& classname::name()                       \
    {                                                          \
        static std::string name##var(value);                   \
        return name##var;                                      \
    }

#define DDSOPCUA_FILE_PATH_MAX_LENGTH (4096)

namespace rti { namespace ddsopcua { namespace utils {

inline std::string normalize_path(const std::string& file_name)
{
    char normalized[DDSOPCUA_FILE_PATH_MAX_LENGTH] = { '\0' };
    if (!RTIOsapiUtility_normalizePath(
                normalized,
                DDSOPCUA_FILE_PATH_MAX_LENGTH,
                file_name.c_str())) {
        RTI_THROW_GATEWAY_EXCEPTION(
                &RTI_OSAPI_UTILITY_LOG_NORMALIZE_PATH_FAILURE_s,
                file_name.c_str());
    }

    return normalized;
}

static std::string executable_path()
{
    char path[DDSOPCUA_FILE_PATH_MAX_LENGTH + 1] = {'\0'};
    if (!RTIOsapiUtility_getSelfDirectoryPath(
            path,
            RTI_OSAPI_STRING_SEQ_STRING_MAX_SIZE)) {
    }

    return std::string(path);
}

class Thread {
public:
    Thread(const std::string& name = std::string("Thread"),
           bool auto_join = false)
            : name_(name),
              auto_join_(auto_join),
              running_(false),
              thread_(nullptr),
              exit_sem_(RTI_OSAPI_SEMAPHORE_KIND_BINARY),
              exception_(false)
    {
    }

    virtual ~Thread()
    {
        if (auto_join_ && running_) {
            try {
                join();
            } catch (...) {
                return;
            }
        }
        if (thread_ != nullptr) {
            RTIOsapiThread_delete(thread_);
        }
    }

    void join(int max_wait_sec = 10)
    {
        if (running_) {
            RTINtpTime max_wait = { 0, 0 };
            max_wait.sec = max_wait_sec;
            exit_sem_.take(&max_wait);
        }

        if (exception_) {
            RTI_THROW_GATEWAY_EXCEPTION(
                    &DDSOPCUA_LOG_ANY_s,
                    exception_message_.c_str());
        }
    }

    void start()
    {
        running_ = true;
        thread_ = RTIOsapiThread_new(
                name_.c_str(),
                RTI_OSAPI_THREAD_PRIORITY_NORMAL,
                RTI_OSAPI_THREAD_OPTION_DEFAULT,
                RTI_OSAPI_THREAD_STACK_SIZE_DEFAULT,
                nullptr,
                run_wrapper,
                this);

        if (thread_ == nullptr) {
            running_ = false;
            RTI_THROW_GATEWAY_EXCEPTION(
                    &DDS_LOG_CREATE_FAILURE_s,
                    name_.c_str());
        }
    }

    virtual void run() = 0;

private:
    static void* run_wrapper(void* args)
    {
        Thread* thread = (Thread*) args;
        thread->exception_ = false;

        try {
            thread->run();
            thread->exit_sem_.give();
        } catch (const std::exception& exception) {
            thread->exception_ = true;
            thread->exception_message_ = exception.what();
        } catch (...) {
            thread->exception_ = true;
            thread->exception_message_ = "Unknown exception";
        }

        thread->running_ = false;

        return nullptr;
    }

private:
    std::string name_;
    bool auto_join_;
    bool running_;
    RTIOsapiThread* thread_;
    rti::core::Semaphore exit_sem_;
    bool exception_;
    std::string exception_message_;
};

}}}  // namespace rti::ddsopcua::utils

#endif /* SERVICE_UTILS_HPP_ */
