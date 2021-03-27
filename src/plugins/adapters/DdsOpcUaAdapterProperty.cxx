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

#include "plugins/adapters/DdsOpcUaAdapterProperty.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

DdsOpcUaAdapterProperty::DdsOpcUaAdapterProperty(
        RTIXMLUTILSObject* xml_root,
        const std::string& service_name)
        : xml_root_(xml_root), service_name_(service_name)
{
}

DdsOpcUaAdapterProperty::DdsOpcUaAdapterProperty(
        const DdsOpcUaAdapterProperty& adapter)
        : xml_root_(adapter.xml_root()), service_name_(adapter.service_name())
{
}

const std::string& DdsOpcUaAdapterProperty::service_name() const
{
    return service_name_;
}

RTIXMLUTILSObject* DdsOpcUaAdapterProperty::xml_root() const
{
    return xml_root_;
}

DdsOpcUaAdapterProperty& DdsOpcUaAdapterProperty::xml_root(
        RTIXMLUTILSObject* xml_root)
{
    xml_root_ = xml_root;
    return *this;
}

}}}  // namespace rti::ddsopcua::adapters
