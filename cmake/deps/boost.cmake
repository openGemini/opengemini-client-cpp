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

find_package(Boost 1.81 QUIET COMPONENTS coroutine url)
if(NOT Boost_FOUND)
    message(STATUS "Download Boost and build it from source")
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_Declare(Boost
        GIT_REPOSITORY https://github.com/boostorg/boost
        GIT_TAG        boost-1.84.0
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(Boost)

    set(OPENGEMINI_EXPORT_BOOST_TARGETS )
    macro(append_boost_targets_recursively TARGET_NAME)
        if (TARGET ${TARGET_NAME})
            get_target_property(TARGET_ORIGIN_NAME ${TARGET_NAME} ALIASED_TARGET)
            list(APPEND OPENGEMINI_EXPORT_BOOST_TARGETS ${TARGET_ORIGIN_NAME})
        endif()

        get_property(DEPEND_TARGETS TARGET ${TARGET_NAME} PROPERTY INTERFACE_LINK_LIBRARIES)
        foreach(DEPEND_TARGET IN LISTS DEPEND_TARGETS)
            append_boost_targets_recursively(${DEPEND_TARGET})
        endforeach()
    endmacro()

    append_boost_targets_recursively("Boost::asio")
    append_boost_targets_recursively("Boost::beast")
    append_boost_targets_recursively("Boost::coroutine")
    append_boost_targets_recursively("Boost::headers")
    append_boost_targets_recursively("Boost::url")

    list(REMOVE_DUPLICATES OPENGEMINI_EXPORT_BOOST_TARGETS)

    foreach(TARGET_NAME IN LISTS OPENGEMINI_EXPORT_BOOST_TARGETS)
        get_property(TARGET_INCLUDE_DIR TARGET ${TARGET_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
        install(
            DIRECTORY   ${TARGET_INCLUDE_DIR}
            DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
    endforeach()

    install(
        TARGETS     ${OPENGEMINI_EXPORT_BOOST_TARGETS}
        EXPORT      ${PROJECT_NAME}Targets
    )
endif()
