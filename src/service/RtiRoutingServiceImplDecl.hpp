/*
 * (c) Copyright, Real-Time Innovations, 2015-2020.
 * All rights reserved.
 *
 * No duplications, whole or partial, manual or electronic, may be made
 * without express written permission.  Any such copies, or
 * revisions thereof, must display this notice unaltered.
 * This code contains trade secrets of Real-Time Innovations, Inc.
 */

#ifndef RTI_DDSOPCUA_ROUTINGSERVICE_IMPL_HPP_
#define RTI_DDSOPCUA_ROUTINGSERVICE_IMPL_HPP_

#ifndef routingservice_service_impl_h

    #include <dds_c/dds_c_common.h>

    #ifdef __cplusplus
extern "C" {
    #endif

struct RTI_RoutingServiceProductInfo {
    char* executable_name;
    const char* shell_executable_name;
    char* product_name;
    char* log_product_name;
    char* default_app_name;
};

struct RTI_RoutingServiceHeapMonitorProperty {
    /* @brief Output directory where files are dumped */
    char* output_dir;
    /* @brief A name to be used in the snapshot files */
    char* snapshot_name;
    /* @brief Period at which snapshots are taken */
    int snapshot_period_millis;
};

    #define RTI_RoutingServiceHeapMonitorProperty_INITIALIZER \
        {                                                     \
            nullptr, nullptr, 0                               \
        }

extern void RTI_RoutingService_execute_heap_snapshot_loop(
        const struct RTI_RoutingServiceHeapMonitorProperty* property,
        int stop_after_sec,
        DDS_Boolean* exit_loop_flag);

extern void RTI_RoutingService_set_product_info(
        struct RTI_RoutingServiceProductInfo* info);

extern void RTI_RoutingService_reset_product_info();

extern DDS_Boolean RTI_RoutingService_initialize_globals();

    #ifdef __cplusplus
} /* extern "C" */
    #endif

#endif  // routingservice_service_impl_h

#endif  // RTI_DDSOPCUA_ROUTINGSERVICE_IMPL_HPP_
