# RTI OPC UA/DDS Gateway

DDS and OPC UA are two of the most prominent connectivity frameworks in the
Industrial Internet of Things. Traditionally, application developers and system
integrators have relied on non-standard custom solutions to integrate DDS and
OPC UA applications. The *RTI® OPC UA/DDS Gateway* leverages the
[OMG OPC UA/DDS Gateway standard](https://www.omg.org/spec/DDS-OPCUA) to
provide a simple generic standards-based solution that provides a transparent
bridge between OPC UA and DDS applications.

Simply configure the *OPC UA/DDS Gateway* to forward data in the Address
Space of OPC UA servers to the DDS Global Data Space or vice versa. The
*Gateway* will automatically instantiate all the required DDS entities and OPC
UA Clients to enable seamless communication with no changes required to
existing OPC UA and *RTI Connext® DDS* applications.

## Cloning Repository

To clone the repository you will need to run `git clone` as follows to download
both the repository and its submodule dependencies:

```bash
git clone --recurse-submodule https://github.com/rticommunity/rtigateway-ddsopcua.git
```

If you forget to clone the repository with `--recurse-submodule`, simply run
the following command to pull all the dependencies:

```bash
git submodule update --init --recursive
```

