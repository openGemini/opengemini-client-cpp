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

message(STATUS "Finding Boost package first before trying to download and build it from source")

FetchContent_Declare(Boost
    GIT_REPOSITORY https://github.com/boostorg/boost
    GIT_TAG        boost-1.84.0
    GIT_PROGRESS   TRUE
    FIND_PACKAGE_ARGS
        1.81
        COMPONENTS coroutine url
)
FetchContent_MakeAvailable(Boost)
