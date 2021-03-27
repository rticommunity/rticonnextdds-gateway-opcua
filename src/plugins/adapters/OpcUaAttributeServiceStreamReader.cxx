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

#include "OpcUaAttributeServiceStreamReader.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

OpcUaAttributeServiceStreamReader::OpcUaAttributeServiceStreamReader(
        const rti::routing::StreamInfo& stream_info,
        rti::opcua::sdk::client::Client& client)
        : stream_info_(stream_info),
          opcua_client_(client),
          mutex_(RTI_OSAPI_SEMAPHORE_KIND_MUTEX),
          data_samples_()
{
    data_samples_.resize(
            1,
            rti::core::native_conversions::cast_from_native<
                    dds::core::xtypes::DynamicType>(*static_cast<DDS_TypeCode*>(
                    stream_info_.type_info().type_representation())));
    info_samples_.resize(1);
    info_samples_.at(0).delegate().native().valid_data = DDS_BOOLEAN_TRUE;
}

OpcUaAttributeServiceStreamReader::~OpcUaAttributeServiceStreamReader()
{
}

std::vector<dds::core::xtypes::DynamicData>& OpcUaAttributeServiceStreamReader::
        samples()
{
    return data_samples_;
}

void OpcUaAttributeServiceStreamReader::take(
        std::vector<dds::core::xtypes::DynamicData*>& sample_seq,
        std::vector<dds::sub::SampleInfo*>& info_seq)
{
    mutex_.take();

    sample_seq.resize(data_samples_.size());
    info_seq.resize(info_samples_.size());

    sample_seq[0] =
            static_cast<dds::core::xtypes::DynamicData*>(&data_samples_[0]);

    info_seq[0] = static_cast<dds::sub::SampleInfo*>(&info_samples_[0]);
}

void OpcUaAttributeServiceStreamReader::return_loan(
        std::vector<dds::core::xtypes::DynamicData*>&,
        std::vector<dds::sub::SampleInfo*>&)
{
    mutex_.give();
}

}}}  // namespace rti::ddsopcua::adapters
