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

# If you installed OpenGeminiCxx to a custom directory,
# you may need to tell CMake where to search the library such as:
#    setting root variable before calling find_package():
#        set(OpenGeminiCxx_ROOT /path/to/the/installation)
#    or specifying the root variable while running cmake:
#        $ cmake -DOpenGeminiCxx_ROOT=/path/to/the/installation
#    or adding the directory to the CMAKE_PREFIX_PATH.
#
# For more usage about find_package() check: https://cmake.org/cmake/help/latest/command/find_package.html

find_package(OpenGeminiCxx REQUIRED)

add_executable(YourApp main.cpp)

# Link OpenGeminiCxx's target against to your target.
target_link_libraries(YourApp PRIVATE OpenGeminiCxx::Client)
