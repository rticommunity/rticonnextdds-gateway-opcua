.. _section-introduction:

Introduction
============

OPC UA and DDS are two of the most prominent connectivity frameworks in the
Industrial Internet of Things.
Traditionally, application developers and system integrators have relied
on non-standard custom solutions to integrate DDS and OPC UA applications.
*RTI® OPC UA/DDS Gateway* leverages the `OMG OPC UA/DDS Gateway standard
<https://www.omg.org/spec/DDS-OPCUA>`_ to provide a simple generic
standards-based solution that provides a transparent bridge to integrate
unmodified OPC UA and DDS applications.

Simply configure the *OPC UA/DDS Gateway* to forward data in the Address
Space of OPC UA servers to the DDS Global Data Space or vice versa. The
*Gateway* will automatically instantiate all the required DDS entities and OPC
UA Clients to enable seamless communication with no changes required to
existing OPC UA and *RTI Connext® DDS* applications.


Implementation Overview
-----------------------

This *OPC UA/DDS Gateway* provides users with the ability to configure OPC UA
subscriptions to monitor the value of Variable Nodes in the address space of an
OPC UA Server, and to map such monitored items to DDS *Topics* using a
compatible type definition. Upon a successful configuration, the *Gateway*
will publish the configured *Topics* over to DDS as data updates become
available in the OPC UA Server.

Moreover, the *Gateway* allows DDS applications to read and write individual
Nodes in the address space of an OPC UA Server through the *Gateway* using the
*RTI Connext® DDS* Request-Reply API.

Lastly, the *Gateway* provides users with the ability to subscribe to DDS
*Topics*, and to map members of such *Topics* to Variable Nodes in the address
space of a remote OPC UA Server. As a result of such configuration, the
*Gateway* will update the Value of the configured Variable Nodes in the remote
OPC UA Server every time it receives a new sample of the configured *Topic*.

.. figure:: ./static/opcua2dds_subscriptions.png
    :figwidth: 100 %
    :alt: OPC UA/DDS Gateway Overview
    :name: FigureOverview
    :align: center

    OPC UA/DDS Gateway Overview

The implementation includes multiple built-in configurations that illustrate
how to use the *OPC UA/DDS Gateway* to interface with DDS applications and a
demo OPC UA Server. These examples and tutorials are described in detail in
:numref:`section-tutorials`.

Available Documentation
-----------------------

The *OPC UA/DDS Gateway* manual includes the following sections:

- :ref:`Installation <section-installation>`, which explains how to build
  and install the *OPC UA/DDS Gateway*.
- :ref:`Usage <section-usage>`, which explains how to run the
  *OPC UA/DDS Gateway*.
- :ref:`Configuration <section-configuration>`, which explains how to configure
  the *OPC UA/DDS Gateway*.
- :ref:`Tutorials <section-tutorials>`, includes a tutorial explaining how to
  run the default built-in configuration for the *OPC UA/DDS Gateway*
  with DDS applications and a demo OPC UA Server.
- :ref:`Release Notes <section-release-notes>` documents the most recent
  additions and the limitations of the current implementation.

.. _section-path-mentioned-in-documentation:

Paths Mentioned in Documentation
--------------------------------

This documentation refers to:

- ``<DDSOPCUA_HOME>`` This refers to the installation directory for
  the *OPC UA/DDS Gateway*.

  You may also see ``$DDSOPCUA_HOME`` or ``%DDSOPCUA_HOME%``, which refers to
  an environment variable set to the installation path.
  Whenever you see ``<DDSOPCUA_HOME>`` used in a path, replace it with your
  installation path.
- ``<CONNEXTDDS_DIR>`` This refers to the installation directory for
  *Connext DDS*.

  You may also see ``$CONNEXTDDS_DIR`` or ``%CONNEXTDDS_DIR%``, which refers to
  an environment variable set to the installation path.
  Whenever you see ``<CONNEXTDDS_DIR>`` used in a path, replace it with your
  installation path.
- ``<RTISHAPESDEMO_HOME>`` This refers to the installation directory for
  *Shapes Demo*. If you have a *Connext DDS Professional* installation,
  ``<RTISHAPESDEMO_HOME>`` can be safely interpreted as ``$CONNEXTDDS_DIR``,
  because *Connext DDS Professional* includes *Shapes Demo*.

  You may also see ``$RTISHAPESDEMO_HOME`` or ``%RTISHAPESDEMO_HOME%``, which
  refers to an environment variable set to the installation path.
  Whenever you see ``<RTISHAPESDEMO_HOME>`` used in a path, replace it with
  your installation path.
