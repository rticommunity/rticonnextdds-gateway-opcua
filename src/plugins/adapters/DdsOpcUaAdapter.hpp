/*
 * (c) 2017-2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#ifndef RTI_OPCUA_ADAPTER_DDSOPCUAADAPTER_HPP_
#define RTI_OPCUA_ADAPTER_DDSOPCUAADAPTER_HPP_

#include <rti/routing/PropertySet.hpp>
#include <rti/routing/adapter/AdapterPlugin.hpp>

#include "plugins/adapters/DdsOpcUaAdapterProperty.hpp"
#include "plugins/adapters/OpcUaConnection.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

class DdsOpcUaAdapter : public rti::routing::adapter::AdapterPlugin {
public:
    DdsOpcUaAdapter(const DdsOpcUaAdapterProperty& adapter_property);

    virtual ~DdsOpcUaAdapter()
    {
    }

    rti::routing::adapter::Connection* create_connection(
            rti::routing::adapter::StreamReaderListener* in_disc_list,
            rti::routing::adapter::StreamReaderListener* out_disc_list,
            const rti::routing::PropertySet& properties) override;

    void delete_connection(rti::routing::adapter::Connection* connection);

    rti::config::LibraryVersion get_version() const;

private:
    DdsOpcUaAdapterProperty adapter_property_;
    rti::config::LibraryVersion library_version_;
};

}}}  // namespace rti::ddsopcua::adapters

#endif  // RTI_OPCUA_ADAPTER_DDSOPCUAADAPTER_HPP_
