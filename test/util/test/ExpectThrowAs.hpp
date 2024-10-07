// Copyright 2024 openGemini Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TEST_UTIL_TEST_EXPECTTHROWAS_HPP
#define TEST_UTIL_TEST_EXPECTTHROWAS_HPP

#include "opengemini/Exception.hpp"

#define EXPECT_THROW_AS(STATEMENT, EXPECTED_ERROR)              \
    try {                                                       \
        STATEMENT;                                              \
        FAIL();                                                 \
    }                                                           \
    catch (const Exception& ex) {                               \
        EXPECT_EQ(ex.UnderlyingError().Code(), EXPECTED_ERROR); \
    }                                                           \
    catch (...) {                                               \
        FAIL();                                                 \
    }

#endif // !TEST_UTIL_TEST_EXPECTTHROWAS_HPP
