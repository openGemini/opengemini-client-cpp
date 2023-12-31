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

