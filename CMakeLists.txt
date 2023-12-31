#
# Copyright 2023 Huawei Cloud Computing Technologies Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.24)

project(OpenGeminiCpp
    VERSION      0.1.0
    DESCRIPTION  "OpenGemini Client SDK For C++"
    HOMEPAGE_URL "https://github.com/openGemini/opengemini-client-cpp"
    LANGUAGES    CXX
)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(FETCHCONTENT_QUIET          OFF)

option(OPENGEMINICPP_BUILD_SHARED_LIBS      "Build shared libraries instead of static ones. \
Only has effect if option OPENGEMINICPP_BUILD_HEADER_ONLY_LIBS is OFF"                                OFF)
option(OPENGEMINICPP_BUILD_HEADER_ONLY_LIBS "Build header-only libraries"                             OFF)
option(OPENGEMINICPP_ENABLE_BUILD_TESTING   "Build unit tests (GoogleTest required)"                  OFF)
option(OPENGEMINICPP_ENABLE_BUILD_EXAMPLE   "Build examples"                                          OFF)
option(OPENGEMINICPP_BUILD_DOCUMENTATION    "Build API documentation (Doxygen required)"              OFF)
option(OPENGEMINICPP_ENABLE_SSL_SUPPORT     "Enable OpenSSL support for using TLS (OpenSSL required)" OFF)

message(STATUS "Finding Boost package first before trying to download and build it from source")
include(FetchContent)
FetchContent_Declare(Boost
    GIT_REPOSITORY https://github.com/boostorg/boost
    GIT_TAG        boost-1.84.0
    GIT_PROGRESS   TRUE
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(Boost)

if(OPENGEMINICPP_ENABLE_SSL_SUPPORT)
    message(STATUS "Finding OpenSSL package")
    find_package(OpenSSL REQUIRED)
endif()

message(STATUS "Generating source code")
add_subdirectory(include)

if(OPENGEMINICPP_ENABLE_BUILD_TESTING)
    message(STATUS "Generating unit test")
    enable_testing()
    add_subdirectory(test)
endif()

if(OPENGEMINICPP_ENABLE_BUILD_EXAMPLE)
    message(STATUS "Generating examples")
    add_subdirectory(examples)
endif()

if (OPENGEMINICPP_BUILD_DOCUMENTATION)
    message(STATUS "Generating documentation")
    add_subdirectory(docs)
endif()
