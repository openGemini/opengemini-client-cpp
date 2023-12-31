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

if(OPENGEMINICPP_BUILD_HEADER_ONLY_LIBS)
    message(STATUS "Generating header-only libraries")

    add_library(Client INTERFACE)
    target_include_directories(Client
        INTERFACE
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(Client
        INTERFACE
            Boost::headers
    )
else()
    set(BUILD_SHARED_LIBS ${OPENGEMINICPP_BUILD_SHARED_LIBS})
    if(BUILD_SHARED_LIBS)
        message(STATUS "Generating shared libraries")
    else()
        message(STATUS "Generating static libraries")
    endif()

    add_library(Client
        opengeminicpp/impl/client.cpp
    )
    target_compile_definitions(Client PUBLIC OPENGEMINICPP_SEPERATE_COMPILATION)
    target_include_directories(Client
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(Client
        PRIVATE
            Boost::headers
    )
endif()

add_library(${PROJECT_NAME}::Client ALIAS Client)
set_target_properties(Client
    PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        DEBUG_POSTFIX d
)

message(STATUS "Generating install target")
include(GNUInstallDirs)

install(
    TARGETS                   Client
    EXPORT                    ${PROJECT_NAME}Targets
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}
)

set(OPENGEMINICPP_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(
    EXPORT      ${PROJECT_NAME}Targets
    DESTINATION ${OPENGEMINICPP_INSTALL_CMAKEDIR}
    NAMESPACE   ${PROJECT_NAME}::
)

install(
    DIRECTORY   opengeminicpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION       ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${OPENGEMINICPP_INSTALL_CMAKEDIR}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION ${OPENGEMINICPP_INSTALL_CMAKEDIR}        
)
