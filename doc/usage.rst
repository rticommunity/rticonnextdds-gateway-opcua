.. _section-usage:

Usage
=====

This chapter explains how to run the *OPC UA/DDS Gateway* once it has been
installed. In particular, it describes:

- How to start the *OPC UA/DDS Gateway* (:numref:`section-starting-gateway`).
- Stopping *OPC UA/DDS Gateway* (:numref:`section-stopping-gateway`).
- Command-line Options (:numref:`section-gateway-command-line-options`).

.. _section-starting-gateway:

Starting the OPC UA/DDS Gateway
-------------------------------
The *OPC UA/DDS Gateway* runs as a separate application. The script to run the
executable is in ``<DDSOPCUA_HOME>/bin``.

To start the *OPC UA/DDS Gateway*, enter:

.. code-block:: console

    $ $DDSOPCUA_HOME/bin/rtiddsopcuagateway [options] -cfgName <cfgName>

For example:

.. code-block:: console

    $ $DDSOPCUA_HOME/bin/rtiddsopcuagateway -cfgName default

:numref:`TableCommandLineOptions` describes the command-line options.

.. _section-stopping-gateway:

Stopping OPC UA/DDS Gateway
--------------------------------
To stop the *OPC UA/DDS Gateway*, press Ctrl-c. The Gateway application
will perform a clean shutdown.

.. _section-gateway-command-line-options:

Command-Line Parameters
-----------------------
The following table describes the command-line parameters for the
*OPC UA/DDS Gateway*. They are all optional, except for ``-cfgName``.

.. |br| raw:: html

   <br />

.. list-table:: RTI OPC UA/DDS Gateway Command-line Options
    :name: TableCommandLineOptions
    :widths: 5 10
    :header-rows: 1

    * - Option
      - Description
    * - ``-cfgFile <file>``
      - Specifies a configuration file to be loaded. See How to Load the XML
        Configuration in :numref:`section-how-to-load-the-xml-configuration`.
    * - ``-cfgName <name>``
      - **Required.** |br|
        Specifies a configuration name. The *OPC UA/DDS Gateway* will look for
        a matching ``<ddsopcua_service>`` tag in the configuration file
        specified via the ``-cfgFile`` parameter and in the default
        configuration files listed in
        :numref:`section-how-to-load-the-xml-configuration`.
    * - ``-help``
      - Displays help information.
    * - ``-verbosity <n>``
      - Controls what type of messages are logged: |br| |br|
        0. Silent |br|
        1. Exceptions (*Connext DDS* and *OPC UA/DDS Gateway*) |br|
        2. Warnings (*OPC UA/DDS Gateway*) |br|
        3. Information (*OPC UA/DDS Gateway*) |br|
        4. Warnings (*Connext DDS* and *OPC UA/DDS Gateway*) |br|
        5. Tracing (*OPC UA/DDS Gateway*) |br|
        6. Tracing (*Connext DDS* and *OPC UA/DDS Gateway*) |br| |br|
        Each verbosity level, ``n``, includes all the levels less than
        ``n``. |br|
        **Default:**  1 (Exceptions)
    * - ``-version``
      - Prints the *OPC UA/DDS Gateway* version number.
