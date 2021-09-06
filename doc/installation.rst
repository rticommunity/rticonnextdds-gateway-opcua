Installation
************

.. _section-installation:

To install the *OPC UA/DDS Gateway* you will need to clone the GitHub
repository, build it, test it, and copy the resulting binaries in your host.

Cloning the Repository
======================

To clone the repository you will need to run ``git clone`` as follows to
download both the repository and its submodule dependencies:

.. code-block:: console

    $ git clone --recurse-submodule https://github.com/rticommunity/rtigateway-ddsopcua.git

If you forget to clone the repository with ``--recurse-submodule``, simply run
the following command to pull all the dependencies:

.. code-block:: console

    $ git submodule update --init --recursive

Building, Testing, and Installing the Gateway
=============================================

Installing Dependencies
-----------------------

To build the *OPC UA/DDS Gateway* you will need to install the following
dependencies:

- `CMake > 3.7 <https://www.cmake.org>`_
- `Python > 3.7 <https://www.python.org/downloads/>`_ (including `Jinja2 <https://pypi.org/project/Jinja2/>`_)
- `RTI® Connext® DDS Professional > 6.1.0 <https://www.rti.com/free-trial>`_

.. note::

    For information on how to download and install *Connext DDS*, please refer
    to its `Getting Started Guide <https://community.rti.com/static/documentation/connext-dds/6.1.0/doc/manuals/connext_dds_professional/getting_started_guide/cpp11/before.html#>`_.

.. note::

    The build system requires the use of *Jinja2* to generate a number of files.
    If you do not have *Jinja2* installed in your system, we recommend you
    install it using ``pip`` from a Python virtual environment and that you
    use the virtual environment to build the *Gateway* as well, which will
    ensure CMake can find *Jinja2* in your path.

    To install *Jinja2* (whether you are running from a virtual environment or
    your host), simply run:

    .. code-block:: console

        $ pip install -r <repository_dir>/resources/python/requirements.txt


Configuring the Build
---------------------

The *Gateway* CMake to generate build infrastructure to build, test, and
install binaries.

To configure the build environment, create a build environment and run CMake as
follows:

.. code-block:: console

    $ mkdir build; cd build
    $ cmake ..

.. note::
    We assume the build directory is one level into the repository directory,
    if you choose to build the Gateway elsewhere, please provide CMake with an
    appropriate path to the source tree (instead of ``..``).

Path to RTI Connext DDS
^^^^^^^^^^^^^^^^^^^^^^^

Above we assume you have installed *Connext DDS* in a default
location (e.g., /opt on Linux or C:\Program Files or Windows). To provide an
alternative Path,use the ```-DCONNEXTDDS_DIR`` argument. Similarly, to
provide a specific architecture, provide the name of the architecture using
the ``-DCONNEXTDDS_ARCH`` argument. For example:

.. code-block:: console

    $ cmake -DCONNEXTDDS_DIR=/custom/path/to/rti_connext_dds-x.y.z \
            -DCONNEXTDDS_ARCH=x64Linux4gcc7.3.0 \
            ..

Building on Windows
^^^^^^^^^^^^^^^^^^^

When building on Windows, you might need to provide CMake with an option to
indicate which compiler you want to use and whether you want to build 32- or
64-bit applications and libraries. For example, the following CMake command
will generate a Visual Studio compatible with Visual Studio 2017 with 64-bit
support:

.. code-block:: console

    $ cmake -G "Visual Studio 15 2017" -A x64 ..

Installation Directory
^^^^^^^^^^^^^^^^^^^^^^

To indicate an installation directory, use `-DCMAKE_INSTALL_PREFIX`. For
example:

.. code-block:: console

    $ cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/dir ..

Building Release or Debug Mode
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You may build the RTI® OPC UA/DDS Gateway in Release or Debug mode. In either
case, we recommend you build shared libraries, and therefore we preset
``BUILD_SHARED_LIBS`` to ON.

To build the Gateway in release mode, use ``-DCMAKE_BUILD_TYPE=`` ``Release``
or ``Debug``. For example:

.. code-block:: console

    cmake -DCMAKE_BUILD_TYPE=Release ..

Summary
^^^^^^^

To build the *OPC UA/DDS Gateway* for a production environment, you can
combine the options above as follows:

.. code-block:: console

    cmake -DCMAKE_BUILD_TYPE=Release \
          -DCONNEXTDDS_DIR=/path/to/rti_connext_dds-6.1.0 \
          -DCONNEXTDDS_ARCH=<architecture> \
          -DCMAKE_INSTALL_PREFIX=/path/to/install/dir/rti_dds_opcua_gateway-6.1.0 \
          ..

Directory Structure
===================

The resulting installation is self-contained. That is, once you have installed
the built results you will have all the necessary executable and libraries to
run the *OPC UA/DDS Gateway*.

.. code-block:: console

      rti_dds_opcua_gateway-<version>/
      ├── bin
      │   ├── rtiddsopcuagatewayapp
      │   ├── tutorial-dds-publisher
      │   ├── tutorial-dds-requester
      │   └── tutorial-opc-ua-server
      ├── include
      │   └── include
      │       └── rti
      │           └── ddsopcua
      │               ├── DdsOpcUaGatewayException.hpp
      │               ├── DdsOpcUaGateway.hpp
      │               ├── DdsOpcUaGatewayProperty.hpp
      │               ├── detail
      │               │   └── DdsOpcUaGatewayImpl.hpp
      │               ├── log
      │               │   └── LogConfig.hpp
      │               └── requester
      │                   ├── DdsRequester.hpp
      │                   └── detail
      │                       ├── DdsRequesterOperations.hpp
      │                       └── OpcUaTypes.hpp
      └── resource
          ├── app
          │   ├── bin
          │   │   └── x64Linux4gcc7.3.0
          │   │       ├── rtiddsopcuagatewayapp
          │   │       ├── tutorial-dds-publisher
          │   │       ├── tutorial-dds-requester
          │   │       └── tutorial-opc-ua-server
          │   └── lib
          │       └── x64Linux4gcc7.3.0
          │           ├── libnddsc.so
          │           ├── libnddscore.so
          │           ├── libnddscpp2.so
          │           ├── libnddscpp.so
          │           ├── libnddsmetp.so
          │           ├── libopen62541.so -> libopen62541.so.1
          │           ├── libopen62541.so.1 -> libopen62541.so.1.1.6
          │           ├── libopen62541.so.1.1.6
          │           ├── librtiapputilsc.so
          │           ├── librticonnextmsgc.so
          │           ├── librticonnextmsgcpp2.so
          │           ├── librtiddsopcuagateway.so
          │           ├── librtidlc.so
          │           ├── librtiroutingservice.so
          │           ├── librtirsinfrastructure.so
          │           └── librtixml2.so
          ├── schema
          │   ├── definitions
          │   │   ├── rti_dds_profiles_definitions.xsd
          │   │   ├── rti_dds_qos_profiles_definitions.xsd
          │   │   ├── rti_dds_topic_types_definitions.xsd
          │   │   ├── rti_dist_logger_definitions.xsd
          │   │   └── rti_service_common_definitions.xsd
          │   └── rti_dds_opcua_service.xsd
          ├── scripts
          │   └── rticommon.sh
          └── xml
              ├── OpcUaBuiltinDataTypes.xml
              └── RTI_DDS_OPCUA_SERVICE.xml