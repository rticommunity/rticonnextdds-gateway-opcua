.. _section-release-notes:

Release Notes
=============

Supported Platforms
^^^^^^^^^^^^^^^^^^^
*RTI OPC UA/DDS Gateway* is supported on the platforms in
:numref:`TableSupportedPlatforms`.

.. list-table:: Supported Platforms
    :name: TableSupportedPlatforms
    :widths: 10 40
    :header-rows: 1

    * - Platform
      - Operating System
    *
      - Linux® (Intel® CPU)
      - All Linux platforms on x64 CPUs in the *RTI Connext DDS Core Libraries
        Release Notes* for the same version number, except for Linux systems
        with a version of GLIBCXX older than 3.4.21, and Wind River® Linux 7
        systems.
    *
      - Windows®
      - All Windows platforms in the *RTI Connext DDS Core Libraries Release
        Notes* for the same version number.

Compatibility
^^^^^^^^^^^^^
For backward compatibility information between *OPC UA/DDS Gateway*
6.0.0 and previous releases, please see the *Migration Guide* on the
`RTI Community portal <https://community.rti.com/Documentation/>`_.

Known Limitations
^^^^^^^^^^^^^^^^^

OPC UA/DDS Gateway Can Only Monitor Node Attributes
'''''''''''''''''''''''''''''''''''''''''''''''''''
The current implementation of the *OPC UA/DDS Gateway* supports only node
attribute monitoring. Future versions of this product may include support for
event monitoring as well.

OPC UA/DDS Gateway Can Only Monitor and Write the Value Attribute of OPC UA Nodes
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
The current implementation of the *OPC UA/DDS Gateway* can only monitor or
write the ``VALUE`` attribute of OPC UA Nodes. Future versions of this product
may include support for monitoring other node attributes.

OPC UA/DDS Gateway Can Only Map an OPC UA Input to a Single DDS Output
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
The current implementation of the *OPC UA/DDS Gateway* can only supports a
single OPC UA Input and a single DDS Output in the context of a Subscription.
Future versions of this product may provide support for multiple Inputs and
Outputs that may be mapped according to configure set of mapping rules.

OPC UA/DDS Gateway Cannot Map ExtensionObject Values
''''''''''''''''''''''''''''''''''''''''''''''''''''
The current implementation of the *OPC UA/DDS Gateway* cannot map items of
ExtensionObject type to DDS Topics. Future versions of this product may provide
support for ExtensionObjects.

OPC UA/DDS Gateway Supports Only Basic Mapping of OPC UA Inputs to DDS Output
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
The current implementation of the *OPC UA/DDS Gateway* maps the value of
monitored items in an OPC UA Input to members of the Topic of a DDS Output
with the same name. Future versions of this product may extend this
functionality to provide custom ways to configure the mapping using XML
syntax, so that any monitored item can be mapped to any member of the DDS
Topic, regardless of their name, as long as the types remain compatible.

OPC UA/DDS Gateway Supports Remote Reading/Writing From Only One OPC UA Server
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
The current implementation of the *OPC UA/DDS Gateway* supports reading and
writing from a single OPC UA Server at a time. While it is possible to
instantiate multiple concurrent subscriptions, the mechanisms that handle
requests and replies from ``rtiddsopcuarequester`` are not prepared to address
different OPC UA Connections.

OPC UA/DDS Gateway Can Only Map a DDS Input to a Single OPC UA Output
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
The current implementation of the *OPC UA/DDS Gateway* can only supports a
single DDS Input and a single OPC UA Output in the context of a Publication.
Future versions of this product may provide support for multiple Inputs and
Outputs that may be mapped according to configure set of mapping rules.
