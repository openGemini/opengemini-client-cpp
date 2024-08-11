//
// Copyright 2024 Huawei Cloud Computing Technologies Co., Ltd.
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
//

#ifndef OPENGEMINI_POINT_HPP
#define OPENGEMINI_POINT_HPP

#include <chrono>
#include <map>
#include <string>
#include <variant>

#include "opengemini/Precision.hpp"

namespace opengemini {

///
/// \~English
/// @brief Holds the point data.
///
/// \~Chinese
/// @brief 点位数据。
///
struct Point {
    using Field = std::variant<double, int64_t, uint64_t, std::string, bool>;
    using Time  = std::chrono::time_point<std::chrono::system_clock,
                                         std::chrono::nanoseconds>;

    std::string                        measurement;
    std::map<std::string, Field>       fields;
    Time                               time;
    std::map<std::string, std::string> tags;
    Precision                          precision{ Precision::Nanosecond };
};

} // namespace opengemini

#endif // !OPENGEMINI_POINT_HPP
