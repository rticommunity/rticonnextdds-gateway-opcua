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

#include <gtest/gtest.h>

#include "rti/ddsopcua/DdsOpcUaGateway.hpp"
#include "rti/ddsopcua/DdsOpcUaGatewayException.hpp"

#include "service/ApplicationProperty.hpp"
#include "serviceApp/Application.hpp"

#include "test/OpcUaTutorialServer.hpp"

namespace rti { namespace ddsopcua { namespace test {

static const char* EXECUTABLE_NAME = "rtiddsopcuagateway";
static DDS_Boolean end = false;

TEST(ServiceAppTests, MissingArgumentParameter)
{
    const char* argv_err[] = { EXECUTABLE_NAME, "-cfgName" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    EXPECT_THROW(
            {
                rti::ddsopcua::service::ApplicationProperty property(
                        2,
                        const_cast<char**>(argv_err),
                        shutdown_hook);
            },
            rti::ddsopcua::GatewayException);
}

TEST(ServiceAppTests, WrongArgument)
{
    const char* argv_err[] = { EXECUTABLE_NAME, "-cfgName", "fakeConfig" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            3,
            const_cast<char**>(argv_err),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    EXPECT_THROW({ application.start(); }, std::exception);
}

TEST(ServiceAppTests, NoArguments)
{
    const char* argv_err[] = { EXECUTABLE_NAME };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            1,
            const_cast<char**>(argv_err),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    EXPECT_THROW({ application.start(); }, rti::ddsopcua::GatewayException);
}

TEST(ServiceAppTests, HelpArgument)
{
    const char* argv_err[] = { EXECUTABLE_NAME, "-help" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            2,
            const_cast<char**>(argv_err),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    ASSERT_FALSE(application.start());
}

TEST(ServiceAppTests, VersionArgument)
{
    const char* argv_err[] = { EXECUTABLE_NAME, "-version" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            2,
            const_cast<char**>(argv_err),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    ASSERT_FALSE(application.start());
}

TEST(ServiceAppTests, ListConfigurations)
{
    const char* argv_err[] = { EXECUTABLE_NAME, "-listConfig" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            2,
            const_cast<char**>(argv_err),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    ASSERT_FALSE(application.start());
}

TEST(ServiceAppTests, NormalRun)
{
    OpcUaTutorialServer server;
    server.start();

    const char* argv[] = { EXECUTABLE_NAME, "-cfgName", "default" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            3,
            const_cast<char**>(argv),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    ASSERT_TRUE(application.start());
}

TEST(ServiceAppTests, CannotConnect)
{
    const char* argv[] = { EXECUTABLE_NAME, "-cfgName", "default" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            3,
            const_cast<char**>(argv),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    EXPECT_THROW({ application.start(); }, dds::core::Exception);
}

TEST(ServiceAppTests, RunWithObjectsPerThread)
{
    OpcUaTutorialServer server;
    server.start();

    const char* argv[] = { EXECUTABLE_NAME,
                           "-cfgName",
                           "default",
                           "-maxObjectsPerThread",
                           "2048" };
    RTI_RoutingServiceRemoteShutdownHook shutdown_hook = { &end, nullptr };
    rti::ddsopcua::service::ApplicationProperty property(
            5,
            const_cast<char**>(argv),
            shutdown_hook);
    rti::ddsopcua::Application application(property);
    ASSERT_TRUE(application.start());
}

}}}  // namespace rti::ddsopcua::test
