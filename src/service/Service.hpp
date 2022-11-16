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

#ifndef RTI_OPCUA_SERVICE_SERVICE_HPP_
#define RTI_OPCUA_SERVICE_SERVICE_HPP_

#include <routingservice/routingservice_service.h>
#include <rti/routing/RoutingService.hpp>
#include <rti/routing/ServiceProperty.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>

#include "config/XmlSupport.hpp"

namespace rti { namespace ddsopcua { namespace service {

/**
 * @brief Provides RAII semantics to the globals initialization
 * and finalization in the Service class.
 * For that to happen, the ServiceGlobalGuards must be instantiated
 * before the creation of XMLSupport and other derived objects.
 */
class ServiceGlobalsGuard {
public:
    ServiceGlobalsGuard();
    ~ServiceGlobalsGuard();
};

class Service {
public:
    /**
     * @brief Construct a new Service object
     *
     * @param property Object with service configuration options
     * @param shutdown Pointer to hook that triggers shutdown of the underlying
     * Routing Service instance.
     */
    Service(const GatewayProperty& property,
            const RTI_RoutingServiceRemoteShutdownHook* shutdown = nullptr);

    /**
     * @brief Destroy the Service object
     */
    ~Service();

    /**
     * @brief Begin execution of the OPC UA/DDS Gateway.
     */
    void start();

    /**
     * @brief Stop execution of the OPC UA/DDS Gateway.
     */
    void stop();

    /**
     * @brief Print out list of available configurations.
     */
    void print_available_configurations();

    /**
     * @brief Configures the global state that must be setup before
     * instantiating a Service object.
     */
    static void initialize_globals();

    /**
     * @brief Finalizes the global state that was setup before the
     * instantiation of the Service object.
     */
    static void finalize_globals();

    /**
     * @brief Provides the number of references to the Service object
     *
     * @return uint32_t Number of references to the Service object
     */
    static uint32_t globals_reference_count();

private:

    /**
     * @brief Configures routing service properties based on the running
     * Service's properties.
     * @param property Routing Service property to be configured
     */
    void initialize_router_property(rti::routing::ServiceProperty& property);

private:
    ServiceGlobalsGuard globals_guard_;
    GatewayProperty property_;
    config::XmlSupport xml_support_;
    rti::routing::ServiceProperty routing_service_property_;
    rti::routing::RoutingService routing_service_;
};

}}}  // namespace rti::ddsopcua::service

#endif  // RTI_OPCUA_SERVICE_SERVICE_HPP
