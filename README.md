# opengemini-client-cpp

![License](https://img.shields.io/badge/license-Apache2.0-green) ![Language](https://img.shields.io/badge/language-C++-blue.svg) [![Version](https://img.shields.io/github/v/tag/opengemini/opengemini-client-cpp?label=release&color=blue)](https://github.com/opengemini/opengemini-client-cpp/releases)

> ⚠️  **This repository is in pre-alpha stage, so not yet feature-complete. And the API has not been frozen, it may be changed on the course of the development.**

English | [简体中文](README_CN.md)

`opengemini-client-cpp` is a C++ client for OpenGemini

---

- [Design Doc](#design-doc)
- [About OpenGemini](#about-opengemini)
- [Prerequisites](#prerequisites)
- [Integration](#integration)
    - [CMake FetchContent](#cmake-fetchcontent)
    - [CMake FindPackage](#cmake-findpackage)
    - [Header-Only](#header-only)
- [Quick Start](#quick-start)
- [Build From Source](#build-from-source)
- [Configuration](#configuration)
- [Contribute](#contribute)

## Design Doc
[OpenGemini Client Design Doc](https://github.com/openGemini/openGemini.github.io/blob/main/src/guide/develop/client_design.md)

## About OpenGemini
OpenGemini is a cloud-native distributed time series database, find more information [here](https://github.com/openGemini/openGemini)

## Prerequisites
- **Building Environment**
    - A compatible operating system: Linux, macOS, Windows
    - A C++ compiler toolset that supports at least C++17 standard: GCC, Clang, MSVC
    - [CMake](https://cmake.org/) 3.12 or later
    - [Doxygen](https://www.doxygen.nl/) (*optional*, for generating documents)
- **Dependencies**
    - [Boost](https://github.com/boostorg/boost) 1.81 or later
    - [{fmt}](https://github.com/fmtlib/fmt)
    - [JSON](https://github.com/nlohmann/json)
    - [OpenSSL](https://github.com/openssl/openssl) (*optional*, for using TLS protocol)
    - [GoogleTest](https://github.com/google/googletest) (*optional*, for building unit tests)

## Integration

> For a whole example project, please check out `examples/integration` directory.

### CMake FetchContent
It is recommended to integrate with OpenGeminiCxx using CMake's [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html).

The library, along with any necessary dependencies(**excluding OpenSSL**), will be downloaded automatically, so you don't need to handle it yourself.

You can including the following lines in your `CMakeLists.txt`:
```cmake
include(FetchContent)
FetchContent_Declare(OpenGeminiCxx
    GIT_REPOSITORY https://github.com/openGemini/opengemini-client-cpp
    GIT_TAG        main
)
FetchContent_MakeAvailable(OpenGeminiCxx)
```

This will export the target `OpenGeminiCxx::Client` which you can link against to your own target like this:
```cmake
add_executable(YourApp main.cpp)
target_link_libraries(YourApp PRIVATE OpenGeminiCxx::Client)
```

### CMake FindPackage
You can also use CMake's `find_package()` function to integrate the library into your project.

This means you need to [build and install](#build-from-source) OpenGeminiCxx on your system first.

Then you can add the following lines in your `CMakeLists.txt`:
```cmake
find_package(OpenGeminiCxx REQUIRED)
```

This will export the target `OpenGeminiCxx::Client` which you can link against to your own target like this:
```cmake
add_executable(YourApp main.cpp)
target_link_libraries(YourApp PRIVATE OpenGeminiCxx::Client)
```

### Header-Only
> **Note:** Although OpenGeminiCxx can be use as a header-only library, we still recommend use the compiled library for improved build times.

Download the [OpenGeminiCxx](https://github.com/openGemini/opengemini-client-cpp/releases), then add `/path/to/opengemini-client-cpp/include` to your project's including directories.

You may also need to link against any necessary dependencies to your program manually, then you can include it directly in your source code with:
```cpp
#include <opengemini/Client.hpp>
```

## Quick Start
> Below are a few examples show how to use some of the main features of the library. For more example code, please check out `examples/usage` directory.
### Query data from the server
```cpp
#include <iostream>

#include <opengemini/Client.hpp>
#include <opengemini/ClientConfigBuilder.hpp>

int main(int argc, char** argv)
{
    // Constructs an openGemini client object.
    opengemini::Client client{ opengemini::ClientConfigBuilder()
                                   // At least one server endpoint needed.
                                   .AppendAddress({ "127.0.0.1", 8086 })
                                   .Finalize() };

    // Performs a query request and print the result, the operation will
    // block until completes or an exception is thrown.
    auto result = client.Query({
        "ExampleDatabase",
        "select * from ExampleMeasurement",
    });
    std::cout << result << std::endl;

    return 0;
}
```

### Write points to server
```cpp
#include <iostream>

#include <opengemini/Client.hpp>
#include <opengemini/ClientConfigBuilder.hpp>

int main(int argc, char** argv)
{
    // Constructs an openGemini client object.
    opengemini::Client client{ opengemini::ClientConfigBuilder()
                                   // At least one server endpoint needed.
                                   .AppendAddress({ "127.0.0.1", 8086 })
                                   .Finalize() };

    // Writes single point to server, the operation will
    // block until completes or an exception is thrown.
    client.Write("ExampleDatabase",
                 {
                     "ExampleMeasurement",
                     {
                         { "Weather", "sunny" },
                         { "Humidity", 521 },
                         { "Temperature", 38.1 },
                     },
                     std::chrono::system_clock::now(),
                 });
    return 0;
}
```

## Build From Source
Make sure all required build tools and dependencies **are installed** on your system, then you can download and build the OpenGeminiCxx library:
```bash
git clone https://github.com/openGemini/opengemini-client-cpp.git
cd opengemini-client-cpp && mkdir build && cd build
cmake ..
make -j
make install
```

If you want to enable TLS support, you can configure with `OPENGEMINI_ENABLE_SSL_SUPPORT` option:
```bash
cmake -DOPENGEMINI_ENABLE_SSL_SUPPORT=ON ..
```

You can also control generation of unit tests with `OPENGEMINI_BUILD_TESTING` option:
```bash
cmake -DOPENGEMINI_BUILD_TESTING=ON ..
```

For details of all the options see [CMake Options](#cmake-options).

## Configuration
### CMake Options
|Option|Description|Default Value|
|:---|:---|:---|
|OPENGEMINI_ENABLE_SSL_SUPPORT|Enable OpenSSL support for using TLS (**OpenSSL required**)|OFF|
|OPENGEMINI_BUILD_DOCUMENTATION|Build API documentation (**Doxygen required**)|OFF|
|OPENGEMINI_BUILD_TESTING|Build unit tests (**GoogleTest required**)|OFF|
|OPENGEMINI_BUILD_SHARED_LIBS|Build shared libraries instead of static ones. Only has effect if option `OPENGEMINI_BUILD_HEADER_ONLY_LIBS` is `OFF`| OFF|
|OPENGEMINI_BUILD_EXAMPLE|Build examples|OFF|
|OPENGEMINI_BUILD_HEADER_ONLY_LIBS|Build as header-only library|OFF|
|OPENGEMINI_GENERATE_INSTALL_TARGET|Generate the install target, should not be set to `ON` if `OPENGEMINI_USE_FETCHCONTENT` is also `ON`|ON<br>(if is root project)|
|OPENGEMINI_USE_FETCHCONTENT|Automatically using FetchContent if dependencies not found|OFF<br>(if is root project)|

## Contribution
Welcome to [join us](CONTRIBUTION.md).
