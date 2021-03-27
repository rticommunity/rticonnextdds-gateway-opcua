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

#include <rti/ddsopcua/DdsOpcUaGatewayException.hpp>

#include "log/LogMsg.hpp"

#include "OpcUaAttributeServiceProcessor.hpp"


namespace rti { namespace ddsopcua { namespace processors {

AttributeServiceSetProcessor::AttributeServiceSetProcessor()
{
}

AttributeServiceSetProcessor::~AttributeServiceSetProcessor()
{
}

void AttributeServiceSetProcessor::on_output_enabled(
        rti::routing::processor::Route& route,
        rti::routing::processor::Output& output)
{
    output_data_ = output.get<dds::core::xtypes::DynamicData>().create_data();
}

void AttributeServiceSetProcessor::on_data_available(
        rti::routing::processor::Route& route)
{
    using namespace dds::core::xtypes;
    using namespace rti::core::xtypes;
    using namespace rti::routing::processor;
    using namespace dds::sub;

    LoanedSamples<DynamicData, SampleInfo> request_samples =
            route.input<DynamicData>("DdsAttributeServiceSet_Request").take();

    for (LoanedSamples<DynamicData, SampleInfo>::iterator request_sample =
                 request_samples.begin();
         request_sample != request_samples.end();
         ++request_sample) {
        SampleInfo& request_info =
                const_cast<SampleInfo&>(request_sample->info());

        if (!request_info.valid()) {
            continue;
        }

        GATEWAYLog_local(
                &DDSOPCUA_LOG_ANY_s,
                "Received AttributeServiceSet request...");

        route.output<DynamicData>("OpcUaAttributeServiceSet_Request")
                .write(request_sample->data());

        LoanedSamples<DynamicData, dds::sub::SampleInfo> reply_samples =
                route.input<DynamicData>("OpcUaAttributeServiceSet_Reply")
                        .take();

        SampleInfo& reply_info =
                const_cast<SampleInfo&>(reply_samples[0].info());
        copy_related_sample_identity(reply_info, request_info);

        route.output<DynamicData>("DdsAttributeServiceSet_Reply")
                .write(reply_samples[0].data(), reply_info);

        GATEWAYLog_local(&DDSOPCUA_LOG_ANY_s, "Sent AttributeServiceSet reply...");
    }
}

void AttributeServiceSetProcessor::copy_related_sample_identity(
        dds::sub::SampleInfo& reply_info,
        const dds::sub::SampleInfo& request_info)
{
    // Copy original_publication_virtual_guid
    DDS_GUID_copy(
            &(reply_info->native().related_original_publication_virtual_guid),
            &(request_info->native().original_publication_virtual_guid));

    // Copy original_publication_virtual_sequence_number
    reply_info->native()
            .related_original_publication_virtual_sequence_number.high =
            request_info->native()
                    .original_publication_virtual_sequence_number.high;
    reply_info->native()
            .related_original_publication_virtual_sequence_number.low =
            request_info->native()
                    .original_publication_virtual_sequence_number.low;
}

AttributeServiceSetProcessorPlugin::AttributeServiceSetProcessorPlugin()
{
}

rti::routing::processor::Processor* AttributeServiceSetProcessorPlugin::
        create_processor(
                rti::routing::processor::Route&,
                const rti::routing::PropertySet&)
{
    return new AttributeServiceSetProcessor();
}

void AttributeServiceSetProcessorPlugin::delete_processor(
        rti::routing::processor::Route&,
        rti::routing::processor::Processor* processor)
{
    delete processor;
}

}}}  // namespace rti::ddsopcua::processors
