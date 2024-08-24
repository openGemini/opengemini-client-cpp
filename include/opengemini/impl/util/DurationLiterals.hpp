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

#ifndef OPENGEMINI_IMPL_UTIL_DURATIONLITERALS_HPP
#define OPENGEMINI_IMPL_UTIL_DURATIONLITERALS_HPP

#include <string>

namespace opengemini::duration_literals {

inline std::string operator"" _second(unsigned long long int second)
{
    return std::to_string(second) + "s";
}

inline std::string operator"" _minute(unsigned long long int minute)
{
    return std::to_string(minute) + "m";
}

inline std::string operator"" _hour(unsigned long long int hour)
{
    return std::to_string(hour) + "h";
}

inline std::string operator"" _day(unsigned long long int day)
{
    return std::to_string(day) + "d";
}

inline std::string operator"" _week(unsigned long long int week)
{
    return std::to_string(week) + "w";
}

} // namespace opengemini::duration_literals

#endif // !OPENGEMINI_IMPL_UTIL_DURATIONLITERALS_HPP
