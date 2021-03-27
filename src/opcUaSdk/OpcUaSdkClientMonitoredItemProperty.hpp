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

#ifndef RTI_OPCUASDK_CLIENTMONITOREDITEM_PROPERTY_HPP_
#define RTI_OPCUASDK_CLIENTMONITOREDITEM_PROPERTY_HPP_

#include <string>
#include <vector>

#include <open62541/client_subscriptions.h>

#include "OpcUaSdkDataTypes.hpp"

namespace rti { namespace opcua { namespace sdk { namespace client {

class NodeAttribute {
public:
    NodeAttribute()
    {
    }

    NodeAttribute(
            const std::string& name,
            const sdk::types::NodeId& node_id,
            const uint32_t& attribute_id)
            : name_(name), node_id_(node_id), attribute_id_(attribute_id)
    {
    }

    NodeAttribute(
            const std::string& name,
            const sdk::types::NodeId& node_id,
            const std::string& attribute_id)
            : name_(name), node_id_(node_id), attribute_id_(attribute_id)
    {
    }

    NodeAttribute(const NodeAttribute& other)
            : name_(other.name()),
              node_id_(other.node_id()),
              attribute_id_(other.attribute_id())
    {
    }

    void name(const std::string& name)
    {
        name_ = name;
    }

    std::string name() const
    {
        return name_;
    }

    void node_id(const types::NodeId& node_id)
    {
        node_id_ = node_id;
    }

    const sdk::types::NodeId& node_id() const
    {
        return node_id_;
    }

    void attribute_id(const uint32_t& attribute_id)
    {
        attribute_id_ = attribute_id;
    }

    void attribute_id(const std::string& attribute_id)
    {
        attribute_id_ = attribute_id;
    }

    uint32_t attribute_id() const
    {
        return attribute_id_.value();
    }


private:
    std::string name_;
    sdk::types::NodeId node_id_;
    sdk::types::AttributeId attribute_id_;
};

class MonitoringProperty {
public:
    MonitoringProperty()
            : sampling_interval_(250), queue_size_(1), discard_oldest_(true)

    {
    }

    MonitoringProperty(const MonitoringProperty& other)
            : sampling_interval_(other.sampling_interval()),
              queue_size_(other.queue_size()),
              discard_oldest_(other.discard_oldest())
    {
    }

    MonitoringProperty& operator=(const MonitoringProperty& other)
    {
        sampling_interval_ = other.sampling_interval();
        queue_size_ = other.queue_size();
        discard_oldest_ = other.discard_oldest();
        return *this;
    }

    void sampling_interval(const double sampling_interval)
    {
        sampling_interval_ = sampling_interval;
    }

    double sampling_interval() const
    {
        return sampling_interval_;
    }

    void queue_size(const uint32_t queue_size)
    {
        queue_size_ = queue_size;
    }

    uint32_t queue_size() const
    {
        return queue_size_;
    }

    void discard_oldest(const bool discard_oldest)
    {
        discard_oldest_ = discard_oldest;
    }

    bool discard_oldest() const
    {
        return discard_oldest_;
    }

private:
    double sampling_interval_;
    uint32_t queue_size_;
    bool discard_oldest_;
};

class MonitoredNodeAttribute : public NodeAttribute {
public:
    MonitoredNodeAttribute()
    {
    }

    MonitoredNodeAttribute(
            const std::string& name,
            const sdk::types::NodeId& node_id,
            const std::string& attribute_id,
            const MonitoringProperty& monitoring_properties)
            : NodeAttribute(name, node_id, attribute_id),
              monitoring_properties_(monitoring_properties)
    {
    }

    MonitoredNodeAttribute& operator=(const MonitoredNodeAttribute& other)
    {
        NodeAttribute::operator=(other);
        monitoring_properties_ = other.monitoring_properties();
        return *this;
    }

    const MonitoringProperty& monitoring_properties() const
    {
        return monitoring_properties_;
    }

    void monitoring_properties(const MonitoringProperty& monitoring_properties)
    {
        monitoring_properties_ = monitoring_properties;
    }

private:
    MonitoringProperty monitoring_properties_;
};


class PublicationProperty {
public:
    PublicationProperty() : field_name_(), sample_selector_name_("*")
    {
    }

    PublicationProperty(
            const std::string& field_name,
            const std::string& sample_selector_name)
            : field_name_(field_name),
              sample_selector_name_(sample_selector_name)
    {
    }

    PublicationProperty(const PublicationProperty& other)
            : field_name_(other.field_name()),
              sample_selector_name_(other.sample_selector_name())
    {
    }

    PublicationProperty& operator=(const PublicationProperty& other)
    {
        field_name_ = other.field_name();
        sample_selector_name_ = other.sample_selector_name();
        return *this;
    }

    void field_name(const std::string& name)
    {
        field_name_ = name;
    }

    std::string field_name() const
    {
        return field_name_;
    }

    void sample_selector_name(const std::string& name)
    {
        sample_selector_name_ = name;
    }

    std::string sample_selector_name() const
    {
        return sample_selector_name_;
    }

private:
    std::string field_name_;
    std::string sample_selector_name_;
};

class PublicationNodeAttribute : public NodeAttribute {
public:
    PublicationNodeAttribute()
    {
    }

    PublicationNodeAttribute(
            const std::string& name,
            const sdk::types::NodeId& node_id,
            const std::string& attribute_id,
            const PublicationProperty& publication_properties)
            : NodeAttribute(name, node_id, attribute_id),
              publication_properties_(publication_properties)
    {
    }

    PublicationNodeAttribute& operator=(const PublicationNodeAttribute& other)
    {
        NodeAttribute::operator=(other);
        publication_properties_ = other.publication_properties();
        return *this;
    }

    PublicationProperty publication_properties() const
    {
        return publication_properties_;
    }

    void publication_properties(
            const PublicationProperty& publication_properties)
    {
        publication_properties_ = publication_properties;
    }

private:
    PublicationProperty publication_properties_;
};


}}}}  // namespace rti::opcua::sdk::client

#endif  // RTI_OPCUASDK_CLIENTMONITOREDITEM_PROPERTY_HPP_
