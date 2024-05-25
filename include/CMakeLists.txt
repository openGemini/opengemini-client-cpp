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

macro(opengemini_target_setting TARGET_NAME TARGET_SCOPE)
    target_compile_definitions(${TARGET_NAME} ${TARGET_SCOPE} ${OPENGEMINI_COMPILE_DEFINITIONS})
    target_include_directories(${TARGET_NAME}
        ${TARGET_SCOPE}
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(${TARGET_NAME}
        ${TARGET_SCOPE}
            Boost::headers
            Boost::coroutine
            fmt::fmt
    )
    if(OPENGEMINI_ENABLE_SSL_SUPPORT)
        target_link_libraries(${TARGET_NAME}
            ${TARGET_SCOPE}
                OpenSSL::SSL
        )
    endif()
endmacro()

if(OPENGEMINI_BUILD_HEADER_ONLY_LIBS)
    add_library(Client INTERFACE)
    opengemini_target_setting(Client INTERFACE)
else()
    add_library(Client
        opengemini/impl/ClientImpl.cpp
        opengemini/impl/ClientConfigBuilder.cpp
        opengemini/impl/ErrorCode.cpp
        opengemini/impl/comm/Context.cpp
        opengemini/impl/http/IHttpClient.cpp
        opengemini/impl/http/HttpClient.cpp
        opengemini/impl/http/HttpsClient.cpp
    )
    opengemini_target_setting(Client PUBLIC)
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

set(OPENGEMINI_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(
    EXPORT      ${PROJECT_NAME}Targets
    DESTINATION ${OPENGEMINI_INSTALL_CMAKEDIR}
    NAMESPACE   ${PROJECT_NAME}::
)

install(
    DIRECTORY   opengemini
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
    INSTALL_DESTINATION ${OPENGEMINI_INSTALL_CMAKEDIR}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION ${OPENGEMINI_INSTALL_CMAKEDIR}        
)
