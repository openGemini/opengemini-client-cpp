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

include_guard()
include(FetchContent)

message(STATUS "Looking for Nlohmann JSON.")
find_package(nlohmann_json ${OPENGEMINI_FIND_PACKAGE_REQUIRED})

if(NOT nlohmann_json_FOUND AND OPENGEMINI_USE_FETCHCONTENT)
    message(STATUS "Nlohmann JSON not found, try using FetchContent instead.")
    FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG        v3.11.3
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()
