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
git clone --recurse-submodule https://github.com/rticommunity/rticonnextdds-gateway-opcua.git
```

If you forget to clone the repository with `--recurse-submodule`, simply run
the following command to pull all the dependencies:

```bash
git submodule update --init --recursive
```

## Building, Testing, and Installing the RTI OPC UA/DDS Gateway

To *RTI® OPC UA/DDS Gateway* you will need to install the following
dependencies:

* [CMake > 3.7](https://cmake.org/) (including Jinja2)
* [Python > 3.7](https://www.python.org/downloads/)
* [RTI Connext DDS Professional > 6.1.0](https://www.rti.com/free-trial)

> The build system requires the use of Jinja2 to generate a number of files.
> If you don't have Jinja2 installed in your system, we recommend you build
> from a Python virtual environment, with Jinja2 installed. To install Jinja2,
> (whether you are running from a virtual environment or your host), simply
> run: `pip install -r <repository_dir>/resources/python/requirements.txt`

### Configuring Build

To configure the build environment, create a build environment and run CMake
as follows:

```bash
mkdir build; cd build
cmake ..
```

> We assume the build directory is one level into the repository directory,
> if you choose to build the Gateway elsewhere, please provide `cmake` with
> an appropriate path to the source tree (alternative to `..`).

#### Path to RTI Connext DDS

Above we assume you have installed RTI Connext® DDS Professional in a default
location (e.g., `/opt` on Linux or `C:\Program Files` or Windows). To provide
an alternative Path,use the `-DCONNEXTDDS_DIR` argument. Similarly, to provide
a specific architecture, provide the name of the architecture using the
`-DCONNEXTDDS_ARCH` argument. For example:

```bash
cmake -DCONNEXTDDS_DIR=/custom/path/to/rti_connext_dds-x.y.z \
      -DCONNEXTDDS_ARCH=x64Linux4gcc7.3.0 \
      ..
```

#### Building on Windows

When building on Windows, you might need to provide CMake with an option to
indicate which compiler you want to use and whether you want to build 32-
or 64-bit applications and libraries. For example, the following CMake command
will generate a Visual Studio compatible with Visual Studio 2017 with 64-bit
support:

```bash
cmake -G "Visual Studio 15 2017" -A x64 ..
```

#### Installation Directory

To indicate an installation directory, use `-DCMAKE_INSTALL_PREFIX`. For
example:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/dir ..
```

#### Building Release or Debug Mode

You may build the *RTI® OPC UA/DDS Gateway* in Release or Debug mode. In either
case, we recommend you build shared libraries, and therefore we preset
`BUILD_SHARED_LIBS` to `ON`.

To build the Gateway in release mode, use `-DCMAKE_BUILD_TYPE=` `Release` or
`Debug`. For example:

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### Summary

To build the  *RTI® OPC UA/DDS Gateway* for a production environment, you can
combine the options above as follows:

```bash
cmake -DCMAKE_BUILD_TYPE=Release
      -DCONNEXTDDS_DIR=/path/to/rti_connext_dds-6.1.0
      -DCONNEXTDDS_ARCH=<architecture>
      -DCMAKE_INSTALL_PREFIX=/path/to/install/dir
      ..
```

### Building the Gateway

After configuring the build, simply run `cmake --build` from the same directory:

```bash
cmake --build .
```

Alternatively, you can use the native build system of your choice, based on
the CMake Generator (e.g., `make` for Makefiles, `ninja`, or Visual Studio).

### Testing the Gateway

To run the *RTI® OPC UA/DDS Gateway* test suite, run `ctest` as follows:

```bash
ctest .
```

### Installing the Gateway

After building the *RTI® OPC UA/DDS Gateway*, you can install it in the
directory you provided via `-DCMAKE_INSTALL_PREDIX`.

If you're using CMake 3.15 or above, you can install the Gateway as follows
from within the build directory:

```bash
cmake --install .
```

> Note that you can override the installation directory using the `--prefix`
> argument.

If you are using an earlier version of CMake, you will need to use the `install`
target of your generator of choice (e.g., `install` for Makefiles or `INSTALL`
for Visual Studio solutions). For example:

```bash
cmake --build . --target install
```
