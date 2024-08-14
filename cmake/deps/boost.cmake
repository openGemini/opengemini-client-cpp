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

include_guard()
include(FetchContent)

message(STATUS "Looking for Boost.")
find_package(Boost 1.81 ${OPENGEMINI_FIND_PACKAGE_REQUIRED} COMPONENTS headers serialization coroutine url)
set(OPENGEMINI_BOOST_HEADER_TARGETS "Boost::headers")

if(NOT Boost_FOUND AND OPENGEMINI_USE_FETCHCONTENT)
    message(STATUS "Boost not found, try using FetchContent instead.")
    set(BOOST_INCLUDE_LIBRARIES asio beast functional coroutine serialization url)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_Declare(Boost
        URL      https://github.com/boostorg/boost/releases/download/boost-1.85.0/boost-1.85.0-cmake.7z
        URL_HASH SHA256=2399fb7b15c84c9dafc4ffb1be69c076da36e541fb960fd971b960c180023f2b
    )
    FetchContent_MakeAvailable(Boost)
    set(OPENGEMINI_BOOST_HEADER_TARGETS "Boost::asio;Boost::beast;Boost::functional")
endif()
