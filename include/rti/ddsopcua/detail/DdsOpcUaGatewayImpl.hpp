/*
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
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

#ifndef RTI_DDSOPCUA_GATEWAY_IMPL_HPP_
#define RTI_DDSOPCUA_GATEWAY_IMPL_HPP_

#include <dds/core/refmacros.hpp>
#include <rti/core/detail/SelfReference.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>

namespace rti { namespace ddsopcua {

namespace service {
class Service;
}

class Application;

class GatewayImpl : public rti::core::detail::RetainableType<GatewayImpl> {
public:
    explicit GatewayImpl(const GatewayProperty& property);
    ~GatewayImpl();

    void start();

    void stop();

    void print_available_configurations();

    static void initialize_globals();
    static void finalize_globals();

private:
    friend class rti::ddsopcua::Application;
    GatewayImpl(const GatewayProperty& property, const void* shutdown_hook);

    rti::ddsopcua::service::Service* impl_;
};

}}  // namespace rti::ddsopcua

#endif  // RTI_DDSOPCUA_GATEWAY_IMPL_HPP_
