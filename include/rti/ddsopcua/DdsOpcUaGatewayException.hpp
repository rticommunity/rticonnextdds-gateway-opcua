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

#ifndef RTI_DDSOPCUA_GATEWAY_EXCEPTION_HPP_
#define RTI_DDSOPCUA_GATEWAY_EXCEPTION_HPP_

#include "routingservice/routingservice_log.h"

#include <dds/core/Exception.hpp>
#include <dds/core/types.hpp>

namespace rti { namespace ddsopcua {

static void initialize_exception_message(
        std::string& message,
        const char* throwing_context,
        const RTILogMessage* log_message,
        va_list ap)
{
    RTIOsapiUtility_snprintf(
            &message[0],
            RTI_LOG_MSG_SIZE_MAX,
            "%s:",
            throwing_context);
    RTIOsapiUtility_vsnprintf(
            &message[0] + strlen(message.c_str()),
            static_cast<int>(RTI_LOG_MSG_SIZE_MAX - strlen(message.c_str())),
            log_message->format,
            ap);
}


class GatewayException : public std::exception {
public:
    GatewayException(
            const char* throwing_context,
            const RTILogMessage* log_message,
            ...)
            : message_(RTI_LOG_MSG_SIZE_MAX, '\0'),
              error_code_(log_message->logNumber)
    {
        va_list ap;
        va_start(ap, log_message);
        initialize_exception_message(
                message_,
                throwing_context,
                log_message,
                ap);
        va_end(ap);
    }

    GatewayException(
            int32_t error_code,
            const char* throwing_context,
            const RTILogMessage* log_message,
            ...)
            : message_(RTI_LOG_MSG_SIZE_MAX, '\0'), error_code_(error_code)
    {
        va_list ap;
        va_start(ap, log_message);
        initialize_exception_message(
                message_,
                throwing_context,
                log_message,
                ap);
        va_end(ap);
    }

    int32_t error_code() const
    {
        return error_code_;
    }

    virtual const char* what() const throw()
    {
        return message_.c_str();
    }

    virtual ~GatewayException() throw()
    {
    }

private:
    GatewayException()
    {
    }

    std::string message_;
    int32_t error_code_;
};

}}  // namespace rti::ddsopcua

#define RTI_THROW_GATEWAY_EXCEPTION(...) \
    throw rti::ddsopcua::GatewayException(RTI_FUNCTION_NAME, __VA_ARGS__)

#define RTI_THROW_GATEWAY_EXCEPTION_W_ERROR_CODE(ERROR_CODE, ...) \
    throw rti::ddsopcua::GatewayException(                        \
            ERROR_CODE,                                           \
            RTI_FUNCTION_NAME,                                    \
            __VA_ARGS__)

#endif  // RTI_DDSOPCUA_GATEWAY_EXCEPTION_HPP_
