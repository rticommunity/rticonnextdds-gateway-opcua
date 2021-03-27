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

#include <rti/core/Exception.hpp>

#include "DdsDynamicDataUtils.hpp"

namespace rti { namespace ddsopcua { namespace conversion {

void check_dynamic_data_return_code(
        DDS_ReturnCode_t retcode,
        const char* message)
{
    rti::core::check_return_code(retcode, message);
    if (retcode == DDS_RETCODE_NO_DATA) {
        // This means that a member name or id doesn't exist or an optional
        // member is not set
        throw dds::core::InvalidArgumentError(
                (std::string(message) + ": member doesn't exist").c_str());
    }
}


}}}  // namespace rti::ddsopcua::conversion
