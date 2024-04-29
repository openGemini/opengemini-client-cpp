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

include(${PROJECT_SOURCE_DIR}/cmake/deps/doxygen.cmake)

set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
set(DOXYGEN_RECURSIVE              NO)
set(DOXYGEN_OUTPUT_LANGUAGE        Chinese)
set(DOXYGEN_OUTPUT_DIRECTORY       docs_cn)
doxygen_add_docs(docs_cn
    ${PROJECT_SOURCE_DIR}/README.md
    ${PROJECT_SOURCE_DIR}/README_EN.md
    ${PROJECT_SOURCE_DIR}/include/opengemini
    ALL
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/include
)

set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README_EN.md)
set(DOXYGEN_RECURSIVE              NO)
set(DOXYGEN_OUTPUT_LANGUAGE        English)
set(DOXYGEN_OUTPUT_DIRECTORY       docs_en)
doxygen_add_docs(docs_en
    ${PROJECT_SOURCE_DIR}/README.md
    ${PROJECT_SOURCE_DIR}/README_EN.md
    ${PROJECT_SOURCE_DIR}/include/opengemini
    ALL
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/include
)
