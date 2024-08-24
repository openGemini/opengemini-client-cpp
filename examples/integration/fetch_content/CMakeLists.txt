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

project(ExampleFetchContent)

# You can set option variables here to control configurations.
#
# Enable TLS support:
#    set(OPENGEMINI_ENABLE_SSL_SUPPORT ON)
# Build OpenGeminiCxx as a shared library:
#    set(OPENGEMINI_BUILD_SHARED_LIBS ON)
#
# For details about all the options, see README file.

# Declare OpenGeminiCxx and make it available
include(FetchContent)
FetchContent_Declare(OpenGeminiCxx
    GIT_REPOSITORY https://github.com/openGemini/opengemini-client-cpp
    GIT_TAG        main
)
FetchContent_MakeAvailable(OpenGeminiCxx)

add_executable(YourApp main.cpp)

# Link OpenGeminiCxx's target against to your target.
target_link_libraries(YourApp PRIVATE OpenGeminiCxx::Client)
