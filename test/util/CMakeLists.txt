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

add_library(TestUtil INTERFACE)
add_library(${PROJECT_NAME}::TestUtil ALIAS TestUtil)
target_include_directories(TestUtil INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(TestUtil INTERFACE ${PROJECT_NAME}::Client)
