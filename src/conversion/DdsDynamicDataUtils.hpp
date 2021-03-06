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

#ifndef RTI_CONVERSIONS_DYNAMICDATAUTILS_HPP_
#define RTI_CONVERSIONS_DYNAMICDATAUTILS_HPP_

#include <dds/core/types.hpp>

namespace rti { namespace ddsopcua { namespace conversion {

/**
 * @brief Checks the value of a DDS_ReturnCode_t and throws the corresponding
 * dds::core::Exception if DDS_ReturnCode_t is different than OK. If the
 * return code is NO_DATA it throws an InvalidArgumentError exception.
 *
 * @param retcode DDS_ReturnCode_t to be checked.
 * @param message Message of the exception to be thrown in case of an error.
 */
void check_dynamic_data_return_code(
        DDS_ReturnCode_t retcode,
        const char* message);

}}}  // namespace rti::ddsopcua::conversion

#endif  // RTI_CONVERSIONS_DYNAMICDATAUTILS_HPP_
