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

#ifndef OPENGEMINI_IMPL_UTIL_TYPETRAITS_HPP
#define OPENGEMINI_IMPL_UTIL_TYPETRAITS_HPP

#include <tuple>
#include <type_traits>

namespace opengemini::util {

template<typename FUNCTION, typename SIGNATURE>
struct IsInvocable : std::false_type { };

template<typename FUNCTION, typename RETURN, typename... ARGS>
struct IsInvocable<FUNCTION, RETURN(ARGS...)> :
    std::is_invocable_r<RETURN, std::decay_t<FUNCTION>, ARGS...> { };

template<typename FUNCTION, typename SIGNATURE>
inline constexpr auto IsInvocable_v = IsInvocable<FUNCTION, SIGNATURE>::value;

template<typename SIGNATURE>
struct HandlerExtraArgs;

template<typename... ARGS>
struct HandlerExtraArgs<void(std::exception_ptr, ARGS...)> {
    using type = std::tuple<ARGS...>;
};

template<typename SIGNATURE>
using HandlerExtraArgs_t = typename HandlerExtraArgs<SIGNATURE>::type;

} // namespace opengemini::util

#endif // !OPENGEMINI_IMPL_UTIL_TYPETRAITS_HPP
