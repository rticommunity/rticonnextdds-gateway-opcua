/*
 * (c) 2018-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include "log/LogMsg.hpp"

#include "ParameterManager.hpp"

namespace rti { namespace ddsopcua { namespace service {

ParameterManager::ParameterManager()
        : executable_name_(""),
          arguments_({})
{
    arguments_.push_back(Argument(
            "cfgFile",
            "",
            ARGUMENT_KIND_STRING,
            false,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "cfgName",
            "",
            ARGUMENT_KIND_STRING,
            true,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "heapSnapshotDir",
            "",
            ARGUMENT_KIND_STRING,
            false,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "heapSnapshotPeriod",
            "",
            ARGUMENT_KIND_INT,
            false,
            true,
            false,
            false));
    arguments_.push_back(
            Argument("help", "", ARGUMENT_KIND_VOID, false, true, true, false));
    arguments_.push_back(Argument(
            "ignoreXsdValidation",
            "",
            ARGUMENT_KIND_VOID,
            false,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "listConfig",
            "",
            ARGUMENT_KIND_VOID,
            false,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "logFormat",
            "",
            ARGUMENT_KIND_STRING,
            false,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "maxObjectsPerThread",
            "",
            ARGUMENT_KIND_INT,
            false,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "verbosity",
            "",
            ARGUMENT_KIND_INT,
            false,
            true,
            false,
            false));
    arguments_.push_back(Argument(
            "version",
            "",
            ARGUMENT_KIND_VOID,
            false,
            true,
            true,
            false));
}


void ParameterManager::parse(int argc, char** argv)
{
    executable_name_ = argv[0];

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        bool valid_argument = false;
        for (int j = 0; j < arguments_.size(); j++) {
            if (REDAString_inCompare(
                        argv[i] + 1,
                        arguments_[j].name.c_str(),
                        static_cast<int>(strlen(argv[i] + 1)))
                == 0) {
                // this is potentially a valid argument
                valid_argument = true;

                // Validate that non-void arguments have a value
                if (arguments_[j].type != ARGUMENT_KIND_VOID
                    && (argc <= i + 1 || argv[i + 1] == nullptr)) {
                    RTI_THROW_GATEWAY_EXCEPTION(
                            &DDSOPCUA_LOG_PARSER_ARGUMENT_MISSING_s,
                            argv[i]);
                }

                switch (arguments_[j].type) {
                case ARGUMENT_KIND_INT: {
                    char* end_ptr = nullptr;
                    arguments_[j].value.int_value =
                            static_cast<int>(strtol(argv[i + 1], &end_ptr, 10));
                    arguments_[j].set = true;
                    i++;
                } break;
                case ARGUMENT_KIND_STRING:
                    arguments_[j].value.string_value = argv[i + 1];
                    arguments_[j].set = true;
                    i++;
                    break;
                case ARGUMENT_KIND_VOID:
                    arguments_[j].set = true;
                    break;
                }

                // Some arguments indicate that parsing must conclude after
                // parsing them (e.g., -help)
                if (arguments_[j].exit) {
                    return;
                }
            }
        }

        if (!valid_argument) {
            // catch-all: unrecognized argument
            RTI_THROW_GATEWAY_EXCEPTION(
                    &DDSOPCUA_LOG_PARSER_FAILURE_ss,
                    "unrecognized argument",
                    argv[i]);
        }
    }
}

ParameterManager::~ParameterManager()
{
}

const std::vector<Argument>& ParameterManager::arguments() const
{
    return arguments_;
}

std::string ParameterManager::executable_name() const
{
    return executable_name_;
}


}}}  // namespace rti::ddsopcua::service
