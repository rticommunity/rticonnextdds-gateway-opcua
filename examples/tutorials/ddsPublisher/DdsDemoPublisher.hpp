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

#ifndef RTI_DDSOPCUA_TUTORIAL_DEMO_PUBLISHER_HPP_
#define RTI_DDSOPCUA_TUTORIAL_DEMO_PUBLISHER_HPP_

#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>

namespace rti { namespace ddsopcua { namespace tutorials {

const size_t MAX_LENGTH = 100;

dds::core::xtypes::StructType create_type()
{
    using namespace dds::core::xtypes;

    StructType mytype("MyType");

    // Scalars
    mytype.add_member(Member("my_boolean", primitive_type<bool>()));
    mytype.add_member(Member("my_byte", primitive_type<DDS_Octet>()));
    mytype.add_member(Member("my_int16", primitive_type<int16_t>()));
    mytype.add_member(Member("my_uint16", primitive_type<uint16_t>()));
    mytype.add_member(Member("my_int32", primitive_type<int32_t>()));
    mytype.add_member(Member("my_uint32", primitive_type<uint32_t>()));
    mytype.add_member(Member("my_int64", primitive_type<rti::core::int64>()));
    mytype.add_member(Member("my_uint64", primitive_type<rti::core::uint64>()));
    mytype.add_member(Member("my_float", primitive_type<float>()));
    mytype.add_member(Member("my_double", primitive_type<double>()));
    mytype.add_member(Member("my_char", primitive_type<DDS_Char>()));
    mytype.add_member(Member("my_string", StringType(MAX_LENGTH)));

    mytype.add_member(
            Member("my_boolean_array",
                   ArrayType(primitive_type<bool>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_byte_array",
                   ArrayType(primitive_type<DDS_Octet>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_int16_array",
                   ArrayType(primitive_type<int16_t>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_uint16_array",
                   ArrayType(primitive_type<uint16_t>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_int32_array",
                   ArrayType(primitive_type<int32_t>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_uint32_array",
                   ArrayType(primitive_type<uint32_t>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_int64_array",
                   ArrayType(primitive_type<rti::core::int64>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_uint64_array",
                   ArrayType(primitive_type<rti::core::uint64>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_float_array",
                   ArrayType(primitive_type<float>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_double_array",
                   ArrayType(primitive_type<double>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_char_array",
                   ArrayType(primitive_type<DDS_Char>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_string_array",
                   ArrayType(StringType(MAX_LENGTH), MAX_LENGTH)));

    // Sequences
    mytype.add_member(
            Member("my_boolean_sequence",
                   SequenceType(primitive_type<bool>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_byte_sequence",
                   SequenceType(primitive_type<DDS_Octet>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_int16_sequence",
                   SequenceType(primitive_type<int16_t>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_uint16_sequence",
                   SequenceType(primitive_type<uint16_t>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_int32_sequence",
                   SequenceType(primitive_type<int32_t>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_uint32_sequence",
                   SequenceType(primitive_type<uint32_t>(), MAX_LENGTH)));
    mytype.add_member(Member(
            "my_int64_sequence",
            SequenceType(primitive_type<rti::core::int64>(), MAX_LENGTH)));
    mytype.add_member(Member(
            "my_uint64_sequence",
            SequenceType(primitive_type<rti::core::uint64>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_float_sequence",
                   SequenceType(primitive_type<float>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_double_sequence",
                   SequenceType(primitive_type<double>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_char_sequence",
                   SequenceType(primitive_type<DDS_Char>(), MAX_LENGTH)));
    mytype.add_member(
            Member("my_string_sequence",
                   SequenceType(StringType(MAX_LENGTH), MAX_LENGTH)));

    return mytype;
}

void update_sample(dds::core::xtypes::DynamicData& sample, int32_t i)
{
    // Boolean
    bool bool_value = i % 2 ? true : false;
    std::vector<DDS_Boolean> bool_vector(MAX_LENGTH, bool_value);
    bool_vector[0] = i % 2 ? DDS_BOOLEAN_FALSE : DDS_BOOLEAN_TRUE;
    bool_vector[MAX_LENGTH - 1] = i % 2 ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;
    sample.value<bool>("my_boolean", bool_value);
    sample.set_values("my_boolean_array", bool_vector);
    sample.set_values("my_boolean_sequence", bool_vector);

    // Byte
    DDS_Octet octet_value = static_cast<DDS_Octet>(i % 128);
    std::vector<DDS_Octet> octet_vector(MAX_LENGTH, octet_value);
    octet_vector[0] = i % 2 ? RTI_UINT8_MIN : RTI_UINT8_MAX;
    octet_vector[MAX_LENGTH - 1] = i % 2 ? RTI_UINT8_MAX : RTI_UINT8_MIN;
    sample.value<DDS_Octet>("my_byte", octet_value);
    sample.set_values("my_byte_array", octet_vector);
    sample.set_values("my_byte_sequence", octet_vector);

    // Int16
    int16_t int16_value = static_cast<int16_t>(
            ((i % 2) == 1) ? (i % RTI_INT16_MAX) : -((i % RTI_INT16_MAX)));
    std::vector<int16_t> int16_vector(MAX_LENGTH, int16_value);
    int16_vector[0] = i % 2 ? RTI_INT16_MIN : RTI_INT16_MAX;
    int16_vector[MAX_LENGTH - 1] = i % 2 ? RTI_INT16_MAX : RTI_INT16_MIN;
    sample.value<int16_t>("my_int16", int16_value);
    sample.set_values("my_int16_array", int16_vector);
    sample.set_values("my_int16_sequence", int16_vector);

    // UInt16
    uint16_t uint16_value = static_cast<uint16_t>(i % RTI_UINT16_MAX);
    std::vector<uint16_t> uint16_vector(MAX_LENGTH, uint16_value);
    uint16_vector[0] = i % 2 ? RTI_UINT16_MIN : RTI_UINT16_MAX;
    uint16_vector[MAX_LENGTH - 1] = i % 2 ? RTI_UINT16_MAX : RTI_UINT16_MIN;
    sample.value<uint16_t>("my_uint16", uint16_value);
    sample.set_values("my_uint16_array", uint16_vector);
    sample.set_values("my_uint16_sequence", uint16_vector);

    // Int32
    int32_t int32_value = static_cast<int32_t>(
            ((i % 2) == 1) ? (i % RTI_INT32_MAX) : -((i % RTI_INT32_MAX)));
    std::vector<int32_t> int32_vector(MAX_LENGTH, int32_value);
    int32_vector[0] = i % 2 ? RTI_INT32_MIN : RTI_INT32_MAX;
    int32_vector[MAX_LENGTH - 1] = i % 2 ? RTI_INT32_MAX : RTI_INT32_MIN;
    sample.value<int32_t>("my_int32", int32_value);
    sample.set_values("my_int32_array", int32_vector);
    sample.set_values("my_int32_sequence", int32_vector);

    // UInt32
    uint32_t uint32_value = static_cast<uint32_t>(i % RTI_UINT32_MAX);
    std::vector<uint32_t> uint32_vector(MAX_LENGTH, uint32_value);
    uint32_vector[0] = i % 2 ? RTI_UINT32_MIN : RTI_UINT32_MAX;
    uint32_vector[MAX_LENGTH - 1] = i % 2 ? RTI_UINT32_MAX : RTI_UINT32_MIN;
    sample.value<uint32_t>("my_uint32", uint32_value);
    sample.set_values("my_uint32_array", uint32_vector);
    sample.set_values("my_uint32_sequence", uint32_vector);

    // Int64
    DDS_LongLong int64_value = static_cast<DDS_LongLong>(
            ((i % 2) == 1) ? (i % RTI_INT64_MAX) : -((i % RTI_INT64_MAX)));
    std::vector<DDS_LongLong> int64_vector(MAX_LENGTH, int64_value);
    int64_vector[0] = i % 2 ? RTI_INT64_MIN : RTI_INT64_MAX;
    int64_vector[MAX_LENGTH - 1] = i % 2 ? RTI_INT64_MAX : RTI_INT64_MIN;
    sample.value<DDS_LongLong>("my_int64", int64_value);
    sample.set_values("my_int64_array", int64_vector);
    sample.set_values("my_int64_sequence", int64_vector);

    // UInt64
    DDS_UnsignedLongLong uint64_value =
            static_cast<DDS_UnsignedLongLong>(i % RTI_UINT64_MAX);
    std::vector<DDS_UnsignedLongLong> uint64_vector(MAX_LENGTH, uint64_value);
    uint64_vector[0] = i % 2 ? RTI_UINT64_MIN : RTI_UINT64_MAX;
    uint64_vector[MAX_LENGTH - 1] = i % 2 ? RTI_UINT64_MAX : RTI_UINT64_MIN;
    sample.value<DDS_UnsignedLongLong>("my_uint64", uint64_value);
    sample.set_values("my_uint64_array", uint64_vector);
    sample.set_values("my_uint64_sequence", uint64_vector);

    // Float
    float float_value = static_cast<float>((i % RTI_INT32_MAX) * 0.5);
    std::vector<float> float_vector(MAX_LENGTH, float_value);
    float_vector[0] = i % 2 ? RTI_FLOAT_MIN : RTI_FLOAT_MAX;
    float_vector[MAX_LENGTH - 1] = i % 2 ? RTI_FLOAT_MAX : RTI_FLOAT_MIN;
    sample.value<float>("my_float", float_value);
    sample.set_values("my_float_array", float_vector);
    sample.set_values("my_float_sequence", float_vector);

    // Double
    double double_value = static_cast<double>((i % RTI_INT32_MAX) * 0.5);
    std::vector<double> double_vector(MAX_LENGTH, double_value);
    double_vector[0] = i % 2 ? RTI_DOUBLE_MIN : RTI_DOUBLE_MAX;
    double_vector[MAX_LENGTH - 1] = i % 2 ? RTI_DOUBLE_MAX : RTI_DOUBLE_MIN;
    sample.value<double>("my_double", double_value);
    sample.set_values("my_double_array", double_vector);
    sample.set_values("my_double_sequence", double_vector);

    // Char
    DDS_Char char_value = (i % 2) ? 'a' : 'b';
    std::vector<DDS_Char> char_vector(MAX_LENGTH, char_value);
    char_vector[0] = i % 2 ? 'A' : 'Z';
    char_vector[MAX_LENGTH - 1] = i % 2 ? 'Z' : 'A';
    sample.value<DDS_Char>("my_char", char_value);
    sample.set_values("my_char_array", char_vector);
    sample.set_values("my_char_sequence", char_vector);

    // String
    std::string string_value = (i % 2) ? "Odd" : "Even";
    std::vector<std::string> string_vector(MAX_LENGTH, string_value);
    string_vector[0] = i % 2 ? "First" : "Last";
    string_vector[MAX_LENGTH - 1] = i % 2 ? "Last" : "First";
    sample.value<std::string>("my_string", string_value);
    for (int i = 0; i < MAX_LENGTH; i++) {
        sample.loan_value("my_string_array")
                .get()
                .value<std::string>(i + 1, string_value);
        sample.loan_value("my_string_sequence")
                .get()
                .value<std::string>(i + 1, string_value);
    }
}

}}}  // namespace rti::ddsopcua::tutorials

#endif  // RTI_DDSOPCUA_TUTORIAL_DEMO_PUBLISHER_HPP_
