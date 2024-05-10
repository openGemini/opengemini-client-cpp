#
# Copyright 2024 Huawei Cloud Computing Technologies Co., Ltd.
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

project(OpenGeminiCxx
    VERSION      0.1.0
    DESCRIPTION  "OpenGemini Client SDK For C++"
    HOMEPAGE_URL "https://github.com/openGemini/opengemini-client-cpp"
    LANGUAGES    CXX
)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(FETCHCONTENT_QUIET          OFF)

set(OPENGEMINI_COMPILE_DEFINITIONS "")

option(OPENGEMINI_BUILD_SHARED_LIBS      "Build shared libraries instead of static ones. \
Only has effect if option OPENGEMINI_BUILD_HEADER_ONLY_LIBS is OFF"                                OFF)
option(OPENGEMINI_BUILD_HEADER_ONLY_LIBS "Build header-only libraries"                             OFF)
option(OPENGEMINI_BUILD_TESTING          "Build unit tests (GoogleTest required)"                  OFF)
option(OPENGEMINI_BUILD_EXAMPLE          "Build examples"                                          OFF)
option(OPENGEMINI_BUILD_DOCUMENTATION    "Build API documentation (Doxygen required)"              OFF)
option(OPENGEMINI_ENABLE_SSL_SUPPORT     "Enable OpenSSL support for using TLS (OpenSSL required)" OFF)

include(${PROJECT_SOURCE_DIR}/cmake/deps/boost.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/deps/fmt.cmake)

if(OPENGEMINI_ENABLE_SSL_SUPPORT)
    include(${PROJECT_SOURCE_DIR}/cmake/deps/openssl.cmake)
    list(APPEND OPENGEMINI_COMPILE_DEFINITIONS "OPENGEMINI_ENABLE_SSL_SUPPORT")
endif()

if(OPENGEMINI_BUILD_HEADER_ONLY_LIBS)
    message(STATUS "Will generating header-only libraries")
else()
    list(APPEND OPENGEMINI_COMPILE_DEFINITIONS "OPENGEMINI_SEPERATE_COMPILATION")

    set(BUILD_SHARED_LIBS ${OPENGEMINI_BUILD_SHARED_LIBS})
    if(BUILD_SHARED_LIBS)
        message(STATUS "Will generating shared libraries")
    else()
        message(STATUS "Will generating static libraries")
    endif()
endif()

message(STATUS "Generating source code")
add_subdirectory(include)

if(OPENGEMINI_BUILD_TESTING)
    message(STATUS "Generating unit test")
    enable_testing()
    add_subdirectory(test)
endif()

if(OPENGEMINI_BUILD_EXAMPLE)
    message(STATUS "Generating examples")
    add_subdirectory(examples)
endif()

if (OPENGEMINI_BUILD_DOCUMENTATION)
    message(STATUS "Generating documentation")
    add_subdirectory(docs)
endif()
