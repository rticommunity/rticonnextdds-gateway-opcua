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

#include <dds/core/refmacros.hpp>
#include <rti/core/detail/SelfReference.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>
#include <rti/ddsopcua/detail/DdsOpcUaGatewayImpl.hpp>

#include "Service.hpp"

namespace rti { namespace ddsopcua {

using namespace service;

GatewayImpl::GatewayImpl(const GatewayProperty& property)
        : impl_(new Service(property))
{
}

GatewayImpl::GatewayImpl(
        const GatewayProperty& property,
        const void* shutdown_hook)
        : impl_(new Service(
                property,
                static_cast<const RTI_RoutingServiceRemoteShutdownHook*>(
                        shutdown_hook)))
{
}

GatewayImpl::~GatewayImpl()
{
}

void GatewayImpl::start()
{
    impl_->start();
}

void GatewayImpl::stop()
{
    impl_->stop();
}

void GatewayImpl::print_available_configurations()
{
    impl_->print_available_configurations();
}


void GatewayImpl::initialize_globals()
{
    Service::initialize_globals();
}

void GatewayImpl::finalize_globals()
{
    Service::finalize_globals();
}

}}  // namespace rti::ddsopcua
