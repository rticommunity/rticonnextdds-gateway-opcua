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
#include <dds/core/xtypes/DynamicType.hpp>

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

void set_dynamic_member_from_string(
    dds::core::xtypes::DynamicData& data,
    const std::string& member_name,
    const std::string& string_value) 
{
    // Check if member exists
    if (!data.member_exists(member_name)) {
        throw std::runtime_error(
            "Member '" + member_name + "' does not exist in type");
    }
     
    const auto type_kind = data.member_info(member_name).member_kind();
    
    try {
        switch (type_kind.underlying()) {
            case dds::core::xtypes::TypeKind::BOOLEAN_TYPE: {
                std::string lower_str = string_value;
                std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
                if (lower_str == "true" || lower_str == "1") {
                    data.value<bool>(member_name, true);
                }
                else if (lower_str == "false" || lower_str == "0") {
                    data.value<bool>(member_name, false);
                }
                else {
                    throw std::runtime_error(
                        "Invalid boolean value: " + string_value);
                }                
                break;
            }
            
            case dds::core::xtypes::TypeKind::UINT_8_TYPE: {
                auto value = static_cast<uint8_t>(std::stoul(string_value));
                data.value<uint8_t>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::INT_16_TYPE: {
                auto value = static_cast<int16_t>(std::stoi(string_value));
                data.value<int16_t>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::UINT_16_TYPE: {
                auto value = static_cast<uint16_t>(std::stoul(string_value));
                data.value<uint16_t>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::INT_32_TYPE: {
                int32_t value = std::stoi(string_value);
                data.value<int32_t>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::UINT_32_TYPE: {
                uint32_t value = std::stoul(string_value);
                data.value<uint32_t>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::INT_64_TYPE: {
                int64_t value = std::stoll(string_value);
                data.value<int64_t>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::UINT_64_TYPE: {
                uint64_t value = std::stoull(string_value);
                data.value<uint64_t>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::FLOAT_32_TYPE: {
                float value = std::stof(string_value);
                data.value<float>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::FLOAT_64_TYPE: {
                double value = std::stod(string_value);
                data.value<double>(member_name, value);
                break;
            }
            
            // case dds::core::xtypes::TypeKind::FLOAT_128_TYPE: {
            //     long double value = std::stold(string_value);
            //     data.value<long double>(member_name, value);
            //     break;
            // }
            
            case dds::core::xtypes::TypeKind::CHAR_8_TYPE: {
                if (string_value.length() != 1) {
                    throw std::runtime_error(
                        "Char type (" + member_name + ") requires exactly one character");
                }
                char value = string_value[0];
                data.value<char>(member_name, value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::STRING_TYPE: {
                data.value<std::string>(member_name, string_value);
                break;
            }
            
            case dds::core::xtypes::TypeKind::ENUMERATION_TYPE: {
                // Try to set enum by string name first, then by numeric value
                try {
                    data.value<std::string>(member_name, string_value);
                } catch (...) {
                    // If string setting fails, try numeric
                    int32_t enum_value = std::stoi(string_value);
                    data.value<int32_t>(member_name, enum_value);
                }
                break;
            }
            
            default: {
                throw std::runtime_error(
                    "Unsupported type kind for member '" + member_name + "': " + 
                        std::to_string(static_cast<int>(type_kind.underlying())));
            }
        }
    }
    catch (const std::invalid_argument& e) {
        throw std::runtime_error(
            "Failed to convert string '" + string_value + 
                "' for member '" + member_name + "': " + e.what());
    }
    catch (const std::out_of_range& e) {
        throw std::runtime_error(
            "Value '" + string_value + "' out of range for member '" + 
                member_name + "': " + e.what());
    }
    catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to set member '" + member_name + "': " + e.what());
    }
}

}}}  // namespace rti::ddsopcua::conversion
