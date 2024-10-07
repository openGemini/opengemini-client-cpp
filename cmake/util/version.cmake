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

function(get_version_from_source RESULT_VERSION)
    set(SOURCE_FILEPATH "${CMAKE_CURRENT_LIST_DIR}/include/opengemini/Version.hpp" )
    file(READ ${SOURCE_FILEPATH} content)

    macro(get_version_number TYPE)
        if (content MATCHES "#define OPENGEMINI_VERSION_${TYPE} ([0-9]+)")
            set(VERSION_${TYPE} ${CMAKE_MATCH_1})
        else()
            message(FATAL_ERROR "Cannot get ${TYPE} version from source: [${SOURCE_FILEPATH}]")
        endif()
    endmacro()

    get_version_number(MAJOR)
    get_version_number(MINOR)
    get_version_number(PATCH)

    set(${RESULT_VERSION} "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" PARENT_SCOPE)
endfunction()
