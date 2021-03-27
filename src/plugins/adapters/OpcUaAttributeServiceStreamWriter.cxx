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

#include <dds/core/xtypes/DynamicData.hpp>

#include "OpcUaAttributeServiceStreamWriter.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

OpcUaAttributeServiceStreamWriter::OpcUaAttributeServiceStreamWriter(
        rti::opcua::sdk::client::Client& client,
        OpcUaAttributeServiceStreamReader* stream_reader)
        : opcua_client_(client), stream_reader_(stream_reader)
{
}

int OpcUaAttributeServiceStreamWriter::write(
        const std::vector<dds::core::xtypes::DynamicData*>& sample_seq,
        const std::vector<dds::sub::SampleInfo*>& info_seq)
{
    using namespace dds::core::xtypes;
    using namespace rti::core::xtypes;
    using namespace rti::opcua;

    LoanedDynamicData request = sample_seq.at(0)->loan_value("request");
    switch (static_cast<types::services::ServiceOperation>(request.get().discriminator_value())) {
    case types::services::ServiceOperation::SERVICE_OPERATION_READ: {
        // Get NodeId
        LoanedDynamicData read = request.get().loan_value("read_request");
        LoanedDynamicData loaned_data = read.get().loan_value("node_id");
        sdk::types::NodeId node_id = ddsopcua::conversion::
                dds_dynamic_data_node_id_to_opc_ua_node_id(loaned_data);
        loaned_data.return_loan();

        // Read
        sdk::types::Variant variant;
        uint32_t status_code = opcua::sdk::types::STATUS_CODE_GOOD;
        std::string reason_phrase = opcua::sdk::types::REASON_PHRASE_GOOD;
        try {
            opcua_client_.read_value_attribute(variant, node_id);
        } catch (const GatewayException& e) {
            status_code = e.error_code();
            reason_phrase = e.what();
        }

        std::vector<DynamicData>& replies = stream_reader_->samples();
        replies.at(0).value<uint32_t>("status_code", status_code);
        replies.at(0).value<std::string>("reason_phrase", reason_phrase);

        LoanedDynamicData read_reply_value =
                replies.at(0).loan_value("reply.read_reply.value");

        ddsopcua::conversion::opc_ua_variant_to_dds_dynamic_data_variant_type(
                read_reply_value.get(),
                variant);

    } break;

    case types::services::ServiceOperation::SERVICE_OPERATION_WRITE: {
        LoanedDynamicData write = request.get().loan_value("write_request");

        // Get NodeId
        LoanedDynamicData loaned_data = write.get().loan_value("node_id");
        sdk::types::NodeId node_id = ddsopcua::conversion::
                dds_dynamic_data_node_id_to_opc_ua_node_id(loaned_data);
        loaned_data.return_loan();

        // Get Value
        loaned_data = write.get().loan_value("value.value");
        sdk::types::Variant variant;
        ddsopcua::conversion::dds_dynamic_data_variant_type_to_opc_ua_variant(
                variant,
                loaned_data);
        loaned_data.return_loan();

        // Get AttributeId
        int32_t attribute_id = write.get().value<int32_t>("attribute_id");
        write.return_loan();

        uint32_t status_code = opcua::sdk::types::STATUS_CODE_GOOD;
        std::string reason_phrase = opcua::sdk::types::REASON_PHRASE_GOOD;
        try {
            opcua_client_.write_value_attribute(node_id, variant);
        } catch (const GatewayException& e) {
            status_code = e.error_code();
            reason_phrase = e.what();
        }

        std::vector<DynamicData>& replies = stream_reader_->samples();
        replies.at(0).value<uint32_t>("status_code", status_code);
        replies.at(0).value<std::string>("reason_phrase", reason_phrase);

        LoanedDynamicData reply = replies.at(0).loan_value("reply");
        reply.get().value<DDS_Octet>("write_reply", 0);
        reply.return_loan();

    } break;
    }

    return 1;
}


}}}  // namespace rti::ddsopcua::adapters
