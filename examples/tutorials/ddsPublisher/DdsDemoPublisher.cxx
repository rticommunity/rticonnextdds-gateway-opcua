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

#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>

#include "DdsDemoPublisher.hpp"

namespace rti { namespace ddsopcua { namespace tutorials {
bool running = true;

void stop_handler(int)
{
    running = false;
}

void run_application(bool& running)
{
    dds::domain::DomainParticipant participant(0);

    dds::core::xtypes::StructType type = create_type();
    dds::topic::Topic<dds::core::xtypes::DynamicData> topic(
            participant,
            "MyTopic",
            type);
    dds::pub::DataWriter<dds::core::xtypes::DynamicData> writer(
            dds::pub::Publisher(participant),
            topic);
    dds::core::xtypes::DynamicData sample(type);

    std::cout << "Publishing MyTopic samples..." << std::endl;

    int32_t i = 0;
    while (running) {
        update_sample(sample, ++i);
        writer.write(sample);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

}}}  // namespace rti::ddsopcua::tutorials

int main()
{
    signal(SIGINT, rti::ddsopcua::tutorials::stop_handler);
    signal(SIGTERM, rti::ddsopcua::tutorials::stop_handler);

    try {
        rti::ddsopcua::tutorials::run_application(
                rti::ddsopcua::tutorials::running);
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }

    dds::domain::DomainParticipant::finalize_participant_factory();
}
