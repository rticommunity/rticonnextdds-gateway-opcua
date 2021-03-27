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

#ifndef RTI_OPCUA_ADAPTER_OPCUAATTRIBUTESERVICESTREAMREADER_HPP_
#define RTI_OPCUA_ADAPTER_OPCUAATTRIBUTESERVICESTREAMREADER_HPP_

#include <vector>

#include <dds/core/xtypes/DynamicData.hpp>
#include <rti/routing/adapter/AdapterPlugin.hpp>

#include "plugins/adapters/DdsOpcUaAdapterProperty.hpp"
#include "opcUaSdk/OpcUaSdkClient.hpp"
#include "service/Utils.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

/**
 * @brief The role of this stream reader is to provide users with a way to
 * access the response from a call to one of the supported services of the
 * OPC UA Attribute Service Set. The response is stored by this stream
 * reader and is updated remotely from the stream writer that is
 * responsible for performing the actual call using the OPC UA SDK.
 */
class OpcUaAttributeServiceStreamReader
        : public rti::routing::adapter::DynamicDataStreamReader {
public:
    /**
     * @brief Constructs a new OpcUaAttributeServiceStreamReader object.
     * @param stream_info Information about the stream to be processed by the
     * stream reader, including its name and type definition.
     * @param client Reference to the OPC UA client associated with the
     * OpcUaConnection.
     */
    OpcUaAttributeServiceStreamReader(
            const rti::routing::StreamInfo& stream_info,
            opcua::sdk::client::Client& client);

    /**
     * @brief Destroys the OpcUaAttributeServiceStreamReader object.
     */
    ~OpcUaAttributeServiceStreamReader();

    /**
     * @brief This method provices access to the most recent response from a
     * call to any supported Service in the AttributeServiceSet.
     *
     * @param sample_seq Provides a vector of DynamicData samples containing
     * the most recent response from an AttributeServiceSet call. The size of
     * the vector is always 1.
     * @param info_seq Provides a vector of SampleInfos for the corresponding
     * samples in sample_seq. The size of the vector is always one and it
     * always marks data as valid.
     */
    void take(
            std::vector<dds::core::xtypes::DynamicData*>& sample_seq,
            std::vector<dds::sub::SampleInfo*>& info_seq) RTI_OVERRIDE;

    /**
     * @brief This method gives the mutex that is taken in the take() method
     * to control concurrent access to the internal data_samples_ vector
     * containing the data.
     * @param sample_seq Unused in this method.
     * @param info_seq Unused in this method.
     */
    void return_loan(
            std::vector<dds::core::xtypes::DynamicData*>&,
            std::vector<dds::sub::SampleInfo*>&) RTI_OVERRIDE;

    /**
     * @brief Provides a reference to the vector containing the most recent
     * response from a call to any supported Service in the
     * AttributeServiceSet. This method may be used to update the value from
     * the context of the StreamWriter that performs the actual
     * AttributeServiceSet call.
     * @return std::vector<dds::core::xtypes::DynamicData>& Reference to the
     * internal vector of samples received. The underlying infrastructure
     * will only use the first value of the vector.
     */
    std::vector<dds::core::xtypes::DynamicData>& samples();

private:
    rti::routing::StreamInfo stream_info_;
    rti::opcua::sdk::client::Client& opcua_client_;
    rti::core::Semaphore mutex_;
    std::vector<dds::core::xtypes::DynamicData> data_samples_;
    std::vector<dds::sub::SampleInfo> info_samples_;
};

}}}  // namespace rti::ddsopcua::adapters

#endif  // RTI_OPCUA_ADAPTER_OPCUAATTRIBUTESERVICESTREAMREADER_HPP_
