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

include(${PROJECT_SOURCE_DIR}/cmake/deps/googletest.cmake)

add_executable(UnitTest
    Client_Test.cpp
    ClientConfigBuilder_Test.cpp
    impl/http/IHttpClient_Test.cpp
    impl/lb/LoadBalancer_Test.cpp
)
add_executable(${PROJECT_NAME}::UnitTest ALIAS UnitTest)

target_link_libraries(UnitTest
    PRIVATE
        ${PROJECT_NAME}::Client
        ${PROJECT_NAME}::TestUtil

        GTest::gmock
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(UnitTest)
