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

#ifndef RTI_DDSOPCUA_GATEWAY_HPP_
#define RTI_DDSOPCUA_GATEWAY_HPP_

#include <dds/core/Reference.hpp>
#include <rti/config/Logger.hpp>

#include <rti/ddsopcua/DdsOpcUaGatewayProperty.hpp>
#include <rti/ddsopcua/detail/DdsOpcUaGatewayImpl.hpp>

namespace rti { namespace ddsopcua {
/**
 * @brief The @product.
 */
class Gateway : public dds::core::Reference<GatewayImpl> {
public:
    typedef dds::core::Reference<GatewayImpl> Base;
    OMG_DDS_REF_TYPE_NOTYPENAME(Gateway, dds::core::Reference, GatewayImpl);

    /**
     * @brief Creates a @product instance
     *
     * @param[in] property
     *            The property to configure @product instance.
     *
     */
    explicit Gateway(const GatewayProperty& property)
            : Base(new GatewayImpl(property))
    {
        this->delegate()->remember_reference(this->delegate());
    }

    explicit Gateway(Base::DELEGATE_REF_T reference) : Base(reference)
    {
        if (this->delegate()) {
            this->delegate()->remember_reference(this->delegate());
        }
    }

    /**
     * @brief Starts @product.
     *
     * This is a non-blocking operation. @product will create its own set of
     * threads to perform its tasks.
     */
    void start()
    {
        this->delegate()->start();
    }

    /**
     * @brief Stops @product.
     *
     * This operation will bloc the instance is fully stopped.
     */
    void stop()
    {
        this->delegate()->stop();
    }

    void print_available_configurations()
    {
        this->delegate()->print_available_configurations();
    }

    static void initialize_globals()
    {
        GatewayImpl::initialize_globals();
    }

    /**
     * @brief Finalizes global resources that @product requires to operate.
     *
     * This operation releases resources specific to @product only. @ndds
     * global state shall be released separately through the
     * DomainParticipantFactory's finalize_instance().
     *
     * This operation should be called by your application only upon exit,
     * after all service instances have been deleted. Calling it at a different
     * time may cause the application to crash.
     *
     * @mtsafety unsafe. Applications are not allowed to call this operation
     * concurrently
     *
     * @throw std::exception
     */
    static void finalize_globals()
    {
        GatewayImpl::finalize_globals();
    }
};

/** \ingroup RTI_RoutingServiceLibModule
 *
 * @brief The singleton type used to configure @product verbosity
 *
 * For configuring other aspects such as output file, print format or
 * RTI Connext specific logging, use rti::config::Logger.
 *
 */
class Logger {
public:
    typedef rti::config::Verbosity Verbosity;
    typedef rti::config::LogCategory LogCategory;
    typedef rti::config::LoggerMode LoggerMode;
    typedef rti::config::PrintFormat PrintFormat;

    static Logger& instance();

    /**
     * @brief Sets the verbosity for the log messages generated at the
     * @product level.
     *
     * @param[in] verbosity
     *            The verbosity of the service logs
     */
    void verbosity(rti::config::Verbosity verbosity);

    /**
     * @brief Getter for the same attribute
     *
     * @see verbosity(rti::config::Verbosity)
     */
    rti::config::Verbosity verbosity();

    virtual ~Logger();

private:
    Logger();

    // Disable copy
    Logger(const Logger&);
    Logger& operator=(const Logger&);
};

}}  // namespace rti::ddsopcua

#endif  // RTI_DDSOPCUA_GATEWAY_HPP_
