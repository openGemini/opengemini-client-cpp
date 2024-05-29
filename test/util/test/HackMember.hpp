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

#ifndef TEST_UTIL_TEST_HACKMEMBER_HPP
#define TEST_UTIL_TEST_HACKMEMBER_HPP

#include <tuple>
namespace opengemini::test {

template<typename T, auto... field>
struct MemberHacker {
    friend auto HackingMember(T&) { return std::make_tuple(field...); }
};

#define OPENGEMINI_TEST_MEMBER_HACKER(T, ...) \
    inline auto HackingMember(T&);            \
    template struct MemberHacker<T, __VA_ARGS__>;

} // namespace opengemini::test

#endif // !TEST_UTIL_TEST_HACKMEMBER_HPP
