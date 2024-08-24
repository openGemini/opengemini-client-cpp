# opengemini-client-cpp

![License](https://img.shields.io/badge/开源许可证-Apache2.0-green) ![language](https://img.shields.io/badge/语言-C++-blue.svg) [![version](https://img.shields.io/github/v/tag/opengemini/opengemini-client-cpp?label=发行版本&color=blue)](https://github.com/opengemini/opengemini-client-cpp/releases)

> ⚠️  **当前仓库仍处于早期开发阶段，功能特性尚未完成。目前提供的API接口不是稳定版本，可能随开发进程有所调整。**

[English](README.md) | 简体中文

`opengemini-client-cpp`是一个用 C++ 语言编写的 OpenGemini 客户端

---

- [设计文档](#设计文档)
- [关于OpenGemini](#关于OpenGemini)
- [前置条件](#前置条件)
- [与项目集成](#与项目集成)
    - [CMake FetchContent](#cmake-fetchcontent)
    - [CMake FindPackage](#cmake-findpackage)
    - [Header-Only](#header-only)
- [快速上手](#快速上手)
- [从源码构建](#从源码构建)
- [配置选项](#配置选项)
- [贡献](#贡献)

## 设计文档
[OpenGemini Client 设计文档](https://github.com/openGemini/openGemini.github.io/blob/main/src/zh/guide/develop/client_design.md)

## 关于 OpenGemini
OpenGemini 是一款云原生分布式时序数据库。获取更多信息，请点击[这里](https://github.com/openGemini/openGemini)

## 前置条件
- **构建环境**
    - 兼容的操作系统：Linux，macOS，Windows
    - 支持C++17（或更新版本）的编译套件: GCC，Clang，MSVC
    - [CMake](https://cmake.org/) 3.12或更高版本
    - [Doxygen](https://www.doxygen.nl/) (*非必选*，用于生成文档)
- **依赖库**
    - [Boost](https://github.com/boostorg/boost) 1.81或更高版本
    - [{fmt}](https://github.com/fmtlib/fmt)
    - [JSON](https://github.com/nlohmann/json)
    - [OpenSSL](https://github.com/openssl/openssl) (*非必选*，用于启用TLS协议支持)
    - [GoogleTest](https://github.com/google/googletest) (*非必选*，用于构建单元测试)


## 与项目集成

> 您可以通过`examples/integration`目录查看完整的示例工程。

### CMake FetchContent
我们推荐使用CMake [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) 来将OpenGeminiCxx集成到您的项目中。

库自身和所有必要的依赖（除了OpenSSL）都会自动下载，不需要手动处理依赖。

可以将下列代码添加至项目的`CMakeLists.txt`中：
```cmake
include(FetchContent)
FetchContent_Declare(OpenGeminiCxx
    GIT_REPOSITORY https://github.com/openGemini/opengemini-client-cpp
    GIT_TAG        main
)
FetchContent_MakeAvailable(OpenGeminiCxx)
```

这将导出目标`OpenGeminiCxx::Client`，您可以将其链接到自己的目标中：
```cmake
add_executable(YourApp main.cpp)
target_link_libraries(YourApp PRIVATE OpenGeminiCxx::Client)
```

### CMake FindPackage
也可以使用CMake `find_package()` 函数来集成到项目中。

这意味着您必须先在系统上[构建并安装](#从源码构建)OpenGeminiCxx。

接着就可以将下列代码添加至`CMakeLists.txt`中：
```cmake
find_package(OpenGeminiCxx REQUIRED)
```

这将导出目标`OpenGeminiCxx::Client`，您可以将其链接到自己的目标中：
```cmake
add_executable(YourApp main.cpp)
target_link_libraries(YourApp PRIVATE OpenGeminiCxx::Client)
```

### Header-Only
> **注意：** 尽管OpenGeminiCxx可以作为纯头文件引用，但我们仍建议使用预编译好的版本以减少构建时间。

下载[OpenGeminiCxx](https://github.com/openGemini/opengemini-client-cpp/releases)，然后将`/path/to/opengemini-client-cpp/include`添加至工程的包含路径中。

然后就可以直接在源码中包含（可能需要手动将必要的依赖库链接到工程中）：
```cpp
#include <opengemini/Client.hpp>
```

## 快速上手
> 下列示例代码简单演示了这个库的一些主要特性，您可以通过`examples/usage`目录查看更多的示例代码。
### 从服务端查询数据
```cpp
#include <iostream>

#include <opengemini/Client.hpp>
#include <opengemini/ClientConfigBuilder.hpp>

int main(int argc, char** argv)
{
    // 构造OpenGemini客户端对象。
    opengemini::Client client{ opengemini::ClientConfigBuilder()
                                    // 需要至少指定一个服务端的端点
                                   .AppendAddress({ "127.0.0.1", 8086 })
                                   .Finalize() };

    // 执行查询请求并直接将结果直接打印。
    // 该操作将会一直阻塞，直到成功完成或抛出异常。
    auto result = client.Query({
        "ExampleDatabase",
        "select * from ExampleMeasurement",
    });
    std::cout << result << std::endl;

    return 0;
}
```

### 向服务端写入点位
```cpp
#include <iostream>

#include <opengemini/Client.hpp>
#include <opengemini/ClientConfigBuilder.hpp>

int main(int argc, char** argv)
{
    // 构造OpenGemini客户端对象。
    opengemini::Client client{ opengemini::ClientConfigBuilder()
                                    // 需要至少指定一个服务端的端点
                                   .AppendAddress({ "127.0.0.1", 8086 })
                                   .Finalize() };

    // 向服务端写入单个点位。
    // 该操作将会一直阻塞，直到成功完成或抛出异常。
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

## 从源码构建
确保所有必要的构建工具和依赖库**已经安装**到您的系统上了，接着就可以下载并构建OpenGeminiCxx：
```bash
git clone https://github.com/openGemini/opengemini-client-cpp.git
cd opengemini-client-cpp && mkdir build && cd build
cmake ..
make -j
make install
```

如果想要启用TLS支持，可以使用选项`OPENGEMINI_ENABLE_SSL_SUPPORT`进行配置：
```bash
cmake -DOPENGEMINI_ENABLE_SSL_SUPPORT=ON ..
```

也可以通过选项`OPENGEMINI_ENABLE_SSL_SUPPORT`来生成单元测试：
```bash
cmake -DOPENGEMINI_BUILD_TESTING=ON ..
```

关于所有配置选项的详细信息，参见[CMake选项](#CMake选项)

## 配置选项
### CMake选项
|选项|描述|默认值|
|:---|:---|:---|
|OPENGEMINI_ENABLE_SSL_SUPPORT|启用TLS支持（**需要OpenSSL**）|OFF|
|OPENGEMINI_BUILD_DOCUMENTATION|构建API文档（**需要Doxygen**）|OFF|
|OPENGEMINI_BUILD_TESTING|构建单元测试（**需要GoogleTest**）|OFF|
|OPENGEMINI_BUILD_SHARED_LIBS|构建为动态库，仅当选项`OPENGEMINI_BUILD_HEADER_ONLY_LIBS`的值为`OFF`时生效| OFF|
|OPENGEMINI_BUILD_EXAMPLE|构建样例代码|OFF|
|OPENGEMINI_BUILD_HEADER_ONLY_LIBS|构建为header-only库|OFF|
|OPENGEMINI_GENERATE_INSTALL_TARGET|生成安装目标，该选项和`OPENGEMINI_USE_FETCHCONTENT`选项的值不能同时为`ON`|ON<br>（当是根项目时）|
|OPENGEMINI_USE_FETCHCONTENT|若无法找到依赖，则自动使用FetchContent|OFF<br>（当是根项目时）|

## 贡献
欢迎[加入我们](CONTRIBUTION.md)。
