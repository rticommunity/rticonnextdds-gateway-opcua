Installation
************

.. _section-installation:

Installation
============

To install the *OPC UA/DDS Gateway* demonstrator, simply extract the evaluation
bundle for your platform on any directory.
For example, on Linux run:

.. code-block:: console

   $ tar xvzf rti_dds_opcua_gateway-<version>-x64Linux.tar.gz

The demonstrator is self-contained. Once you have extracted the bundle, you
will have all the necessary executable and libraries to run the *OPC UA/DDS
Gateway*.

.. code-block:: console

      rti_dds_opcua_gateway-<version>/
      ├── bin
      │   ├── rtiddsdemopublisher
      │   ├── rtiddsopcuagateway
      │   ├── rtiddsopcuarequester
      │   └── rtiopcuademoserver
      └── resource
          ├── app
          │   ├── bin
          │   │   └── x86_64Linux
          │   │       ├── rtiddsdemopublisher
          │   │       ├── rtiddsopcuagateway
          │   │       ├── rtiddsopcuarequester
          │   │       └── rtiopcuademoserver
          │   └── lib
          │       └── x86_64Linux
          │           ├── libnddscore.so
          │           ├── libnddscpp2.so
          │           ├── libnddsc.so
          │           ├── libnddsmetp.so
          │           ├── libopen62541.so -> libopen62541.so.0
          │           ├── libopen62541.so.1 -> libopen62541.so.1.0.0
          │           ├── libopen62541.so.1.0.0
          │           ├── librticonnextmsgc.so
          │           ├── librtidlc.so
          │           ├── librtiroutingservice.so
          │           └── librtixml2.so
          ├── schema
          │   ├── definitions
          │   │   ├── rti_dds_profiles_definitions.xsd
          │   │   ├── rti_dds_qos_profiles_definitions.xsd
          │   │   ├── rti_dds_topic_types_definitions.xsd
          │   │   ├── rti_dist_logger_definitions.xsd
          │   │   ├── rti_recorder_common_definitions.xsd
          │   │   └── rti_service_common_definitions.xsd
          │   └── rti_dds_opcua_service.xsd
          ├── scripts
          │   ├── rticommon_config.sh
          │   └── rticommon.sh
          └── xml
              └── RTI_DDS_OPCUA_SERVICE.xml
