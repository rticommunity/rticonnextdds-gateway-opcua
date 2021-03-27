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

#ifndef RTI_DDSOPCUA_LOG_CONFIG_HPP_
#define RTI_DDSOPCUA_LOG_CONFIG_HPP_

#include <cstdarg>
#include <cstdio>

#include <ndds/log/log_common.h>
#include <rti/config/Logger.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#define RTI_SUBMODULE_MASK_ALL (0xFFFF)

namespace rti { namespace ddsopcua { namespace log {

class LogConfig {
public:
    static LogConfig& instance()
    {
        static LogConfig singleton;
        return singleton;
    }

    RTILogBitmap instrumentation_mask() const
    {
        return instrumentation_mask_;
    }

    LogConfig& instrumentation_mask(RTILogBitmap logmask)
    {
        instrumentation_mask_ = logmask;
        return *this;
    }

    rti::config::Verbosity verbosity()
    {
        return static_cast<rti::config::Verbosity::type>(instrumentation_mask_);
    }

    void verbosity(rti::config::Verbosity verbosity)
    {
        instrumentation_mask(static_cast<RTILogBitmap>(verbosity.underlying()));
    }

    ~LogConfig()
    {
    }

    static RTILogBitmap mask_all()
    {
        return RTI_SUBMODULE_MASK_ALL;
    }

    static RTILogBitmap mask_none()
    {
        return 0;
    }

protected:
    // Constructor is private: this class is a singleton
    LogConfig() : instrumentation_mask_(RTI_LOG_BIT_EXCEPTION)
    {
        RTILog_appendPrintMask(RTI_LOG_PRINT_BIT_AT_METHOD);
    }

    RTILogBitmap instrumentation_mask_;
};

inline rti::config::Verbosity dds_verbosity_from_level(int32_t level)
{
    switch (level) {
    case 6:
        return rti::config::Verbosity::STATUS_REMOTE;
    case 5:
        /* no break */
    case 4:
        return rti::config::Verbosity::WARNING;
    case 3:
        /* no break */
    case 2:
        /* no break */
    case 1:
        return rti::config::Verbosity::EXCEPTION;
    case 0:
        return rti::config::Verbosity::SILENT;
    default:
        return rti::config::Verbosity::EXCEPTION;
    }
}

inline rti::config::Verbosity service_verbosity_from_level(int32_t level)
{
    RTILogBitmap native_verbosity = RTI_LOG_BIT_SILENCE;

    switch (level) {
        /* no break */
    case 6:
        native_verbosity |= RTI_LOG_BIT_CONTENT;
        /* no break */
    case 5:
        native_verbosity |= RTI_LOG_BIT_PERIODIC;
        /* no break */
    case 4:
        /* no break */
        native_verbosity |= RTI_LOG_BIT_REMOTE;
    case 3:
        native_verbosity |= RTI_LOG_BIT_LOCAL;
        /* no break */
    case 2:
        native_verbosity |= RTI_LOG_BIT_WARN;
        /* no break */
    case 1:
        native_verbosity |= RTI_LOG_BIT_EXCEPTION;
        break;
    case 0:
        break;
    default:
        native_verbosity = RTI_LOG_BIT_EXCEPTION;
        break;
    }

    return static_cast<rti::config::Verbosity::type>(native_verbosity);
}


}}}  // namespace rti::ddsopcua::log


/*
 * --- C-like logger macros ---------------------------------------------------
 */
#define GATEWAYLogMessage_log(INSTRUMENT_BIT, ...)      \
    RTILogMessage_log(                                  \
            (MODULE_LOG_CONFIG).instrumentation_mask(), \
            RTI_SUBMODULE_MASK_ALL,                     \
            RTI_SUBMODULE_MASK_ALL,                     \
            RTI_LOG_PRINT_FORMAT_MASK_ALL,              \
            (INSTRUMENT_BIT),                           \
            DEFINE_MODULE_NUMBER(MODULE_NS),            \
            __VA_ARGS__)

#define GATEWAYLog_exception(...) \
    GATEWAYLogMessage_log(RTI_LOG_BIT_EXCEPTION, __VA_ARGS__)

#define GATEWAYLog_warn(...) \
    GATEWAYLogMessage_log(RTI_LOG_BIT_WARN, __VA_ARGS__)

#define GATEWAYLog_local(...) \
    GATEWAYLogMessage_log(RTI_LOG_BIT_LOCAL, __VA_ARGS__)

#define GATEWAYLog_content(...) \
    GATEWAYLogMessage_log(RTI_LOG_BIT_CONTENT, __VA_ARGS__)

#define GATEWAYLog_remote(...) \
    GATEWAYLogMessage_log(RTI_LOG_BIT_REMOTE, __VA_ARGS__)


#define GATEWAYLog_any(INSTRUMENT_BIT, ...)                               \
    if ((((MODULE_LOG_CONFIG).instrumentation_mask()) & (INSTRUMENT_BIT)) \
        && ((RTI_SUBMODULE_MASK_ALL) & (RTI_SUBMODULE_MASK_ALL)))

#define GATEWAYLog_flushActivityContextWithAction(INSTRUMENT_BIT, ACTION) \
    if (((MODULE_LOG_CONFIG).instrumentation_mask() & ((INSTRUMENT_BIT))) \
        && RTI_SUBMODULE_MASK_ALL)                                        \
        RTILog_printContextAndMsg(INSTRUMENT_BIT, "", &RTI_LOG_ANY_s, (ACTION));

#define GATEWAYLog_flushActivityContext(INSTRUMENT_BIT) \
    GATEWAYLog_logMessage((INSTRUMENT_BIT), &RTI_LOG_ANY_s, "");

#endif  // RTI_DDSOPCUA_LOG_CONFIG_HPP_
