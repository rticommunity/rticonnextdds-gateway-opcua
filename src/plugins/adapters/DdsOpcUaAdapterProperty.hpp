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

#ifndef RTI_OPCUA_ADAPTER_DDSOPCUA_ADAPTER_PROPERTIES_HPP_
#define RTI_OPCUA_ADAPTER_DDSOPCUA_ADAPTER_PROPERTIES_HPP_

#include "config/XmlSupport.hpp"

namespace rti { namespace ddsopcua { namespace adapters {

class DdsOpcUaAdapterProperty {
public:
    /**
     * @brief Constructor of the DdsOpcUaAdapterProperty class
     * Keeps a reference to the root of the XML DOM containing the service
     * configuration, as well as a copy of the service name.
     * @param xml_root Reference to the root of the XML DOM containing the
     * service configuration
     * @param service_name Name of the service instance
     */
    DdsOpcUaAdapterProperty(
            RTIXMLUTILSObject* xml_root,
            const std::string& service_name);

    /**
     * Copy constructor
     * @param adapter Reference to adapter to be copied
     */
    DdsOpcUaAdapterProperty(const DdsOpcUaAdapterProperty& adapter);

    /**
     * Returns a const reference to the service name of the
     * DdsOpcUaAdapterProperty instance.
     * @return const reference to the service name
     */
    const std::string& service_name() const;

    /**
     * Returns a pointer to the XML Root of the DdsOpcUaAdapterProperty
     * @return Pointer to the root of the DdsOpcUaAdapterProperty
     */
    RTIXMLUTILSObject* xml_root() const;

    /**
     * Sets XML root of the DdsOpcUaAdapterProperty
     * @param xml_root Pointer to the new root of DdsOpcUaAdapterProperty
     * @return Reference to the updated object
     */
    DdsOpcUaAdapterProperty& xml_root(RTIXMLUTILSObject* xml_root);

private:
    RTIXMLUTILSObject* xml_root_;
    const std::string service_name_;
};

}}}  // namespace rti::ddsopcua::adapters

#endif  // RTI_OPCUA_ADAPTER_DDSOPCUA_ADAPTER_PROPERTIES_HPP_