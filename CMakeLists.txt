# Copyright 2024 openGemini Authors
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

cmake_minimum_required(VERSION 3.12)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/util/version.cmake)
get_version_from_source(OPENGEMINI_PROJECT_VERSION)
message(STATUS "OpenGeminiCxx version: ${OPENGEMINI_PROJECT_VERSION}")

project(OpenGeminiCxx
    VERSION      ${OPENGEMINI_PROJECT_VERSION}
    DESCRIPTION  "OpenGemini Client SDK For C++"
    HOMEPAGE_URL "https://github.com/openGemini/opengemini-client-cpp"
    LANGUAGES    CXX
)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(FETCHCONTENT_QUIET          OFF)
set(BUILD_SHARED_LIBS           ${OPENGEMINI_BUILD_SHARED_LIBS})

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message(STATUS "OpenGeminiCxx is the top level project.")
    set(OPENGEMINI_IS_TOP_LEVEL_PROJECT ON)
endif()
set(OPENGEMINI_COMPILE_DEFINITIONS "")

option(OPENGEMINI_BUILD_SHARED_LIBS      "Build shared libraries instead of static ones. \
Only has effect if option <OPENGEMINI_BUILD_HEADER_ONLY_LIBS> is OFF"                                  OFF)
option(OPENGEMINI_BUILD_HEADER_ONLY_LIBS "Build header-only libraries"                                 OFF)
option(OPENGEMINI_BUILD_TESTING          "Build unit tests (GoogleTest required)"                      OFF)
option(OPENGEMINI_BUILD_EXAMPLE          "Build examples"                                              OFF)
option(OPENGEMINI_BUILD_DOCUMENTATION    "Build API documentation (Doxygen required)"                  OFF)
option(OPENGEMINI_ENABLE_SSL_SUPPORT     "Enable OpenSSL support for using TLS (OpenSSL required)"     OFF)

set(_OPENGEMINI_GENERATE_INSTALL_TARGET ${OPENGEMINI_IS_TOP_LEVEL_PROJECT})
if(OPENGEMINI_USE_FETCHCONTENT)
    set(_OPENGEMINI_GENERATE_INSTALL_TARGET OFF)
endif()
option(OPENGEMINI_GENERATE_INSTALL_TARGET "Generate the install target" ${_OPENGEMINI_GENERATE_INSTALL_TARGET})
unset(_OPENGEMINI_GENERATE_INSTALL_TARGET)

set(_OPENGEMINI_USE_FETCHCONTENT OFF)
if(NOT OPENGEMINI_IS_TOP_LEVEL_PROJECT)
    set(_OPENGEMINI_USE_FETCHCONTENT ON)
endif()
option(OPENGEMINI_USE_FETCHCONTENT "Automatically using FetchContent if dependencies not found" ${_OPENGEMINI_USE_FETCHCONTENT})
unset(_OPENGEMINI_USE_FETCHCONTENT)

if(OPENGEMINI_USE_FETCHCONTENT AND OPENGEMINI_GENERATE_INSTALL_TARGET)
    message(FATAL_ERROR "Option <OPENGEMINI_USE_FETCHCONTENT> and <OPENGEMINI_GENERATE_INSTALL_TARGET> \
                         should not be set to ON at the same time. Consider setting one of them to OFF.")
endif()

set(OPENGEMINI_FIND_PACKAGE_REQUIRED "REQUIRED")
if(OPENGEMINI_USE_FETCHCONTENT)
    set(OPENGEMINI_FIND_PACKAGE_REQUIRED "")
endif()

include(${PROJECT_SOURCE_DIR}/cmake/deps/boost.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/deps/fmt.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/deps/nlohmann_json.cmake)

if(OPENGEMINI_ENABLE_SSL_SUPPORT)
    include(${PROJECT_SOURCE_DIR}/cmake/deps/openssl.cmake)
    list(APPEND OPENGEMINI_COMPILE_DEFINITIONS "OPENGEMINI_ENABLE_SSL_SUPPORT")
endif()

if(OPENGEMINI_BUILD_HEADER_ONLY_LIBS)
    message(STATUS "Will generating header-only libraries")
else()
    list(APPEND OPENGEMINI_COMPILE_DEFINITIONS "OPENGEMINI_SEPARATE_COMPILATION")

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
    add_subdirectory(examples/usage)
endif()

if (OPENGEMINI_BUILD_DOCUMENTATION)
    message(STATUS "Generating documentation")
    add_subdirectory(docs)
endif()
