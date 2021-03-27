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

#ifndef RTI_OPCUASDK_CLIENTSUBSCRIPTIONPROPERTY_HPP_
#define RTI_OPCUASDK_CLIENTSUBSCRIPTIONPROPERTY_HPP_

#include <open62541/client_subscriptions.h>

#include <dds/core/Duration.hpp>

namespace rti { namespace opcua { namespace sdk { namespace client {

class SubscriptionProperty {
public:
    SubscriptionProperty()
            : subscription_settings_(UA_CreateSubscriptionRequest_default())
    {
    }

    ~SubscriptionProperty()
    {
    }

    void requested_publishing_interval(const double publishing_interval)
    {
        subscription_settings_.requestedPublishingInterval =
                publishing_interval;
    }

    double requested_publishing_interval() const
    {
        return subscription_settings_.requestedPublishingInterval;
    }

    void requested_max_keep_alive_count(const uint32_t max_keep_alive_count)
    {
        subscription_settings_.requestedMaxKeepAliveCount =
                max_keep_alive_count;
    }

    uint32_t requested_max_keep_alive_count() const
    {
        return subscription_settings_.requestedMaxKeepAliveCount;
    }

    void requested_lifetime_count(const uint32_t lifetime_count)
    {
        subscription_settings_.requestedLifetimeCount = lifetime_count;
    }

    uint32_t requested_lifetime_count() const
    {
        return subscription_settings_.requestedLifetimeCount;
    }

    void max_notifications_per_publish(const uint32_t notifications_per_publish)
    {
        subscription_settings_.maxNotificationsPerPublish =
                notifications_per_publish;
    }

    uint32_t max_notifications_per_publish() const
    {
        return subscription_settings_.maxNotificationsPerPublish;
    }

    void publishing_enabled(const bool publishing_enabled)
    {
        subscription_settings_.publishingEnabled = publishing_enabled;
    }

    bool publishing_enabled() const
    {
        return subscription_settings_.publishingEnabled;
    }

    void priority(const uint8_t priority)
    {
        subscription_settings_.priority = priority;
    }

    uint8_t priority() const
    {
        return subscription_settings_.priority;
    }

private:
    UA_CreateSubscriptionRequest subscription_settings_;
};

}}}}  // namespace rti::opcua::sdk::client

#endif  // RTI_OPCUASDK_CLIENTSUBSCRIPTIONPROPERTY_HPP_
