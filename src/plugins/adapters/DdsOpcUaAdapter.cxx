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

#include <dds/core/types.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>

#include "plugins/adapters/DdsOpcUaAdapter.hpp"
#include "plugins/adapters/OpcUaConnection.hpp"
#include "config/XmlEntities.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

DdsOpcUaAdapter::DdsOpcUaAdapter(const DdsOpcUaAdapterProperty& property)
        : adapter_property_(property),
          library_version_(
                  rti::config::product_version().major_version(),
                  rti::config::product_version().minor_version(),
                  rti::config::product_version().release_version(),
                  rti::config::product_version().revision_version())
{
}

rti::routing::adapter::Connection* DdsOpcUaAdapter::create_connection(
        rti::routing::adapter::StreamReaderListener*,
        rti::routing::adapter::StreamReaderListener*,
        const rti::routing::PropertySet& connection_property)
{
    return new OpcUaConnection(adapter_property_, connection_property);
}

void DdsOpcUaAdapter::delete_connection(
        rti::routing::adapter::Connection* connection)
{
    delete connection;
}

rti::config::LibraryVersion DdsOpcUaAdapter::get_version() const
{
    return library_version_;
}

}}}  // namespace rti::ddsopcua::adapters
