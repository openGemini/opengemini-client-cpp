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

#ifndef OPENGEMINI_UTIL_PREPROCESSOR_HPP
#define OPENGEMINI_UTIL_PREPROCESSOR_HPP

#ifdef OPENGEMINI_SEPARATE_COMPILATION
#    define OPENGEMINI_INLINE_SPECIFIER
#else
#    define OPENGEMINI_INLINE_SPECIFIER inline
#endif // OPENGEMINI_SEPARATE_COMPILATION

#define OPENGEMINI_PF(ARG) std::forward<decltype(ARG)>(ARG)

#if __cplusplus >= 202002L
#    define OPENGEMINI_CONSTEXPR_SINCE_CXX20 constexpr
#    define OPENGEMINI_CONSTEVAL_SINCE_CXX20 consteval
#else // (__cplusplus >= 202002L)
#    define OPENGEMINI_CONSTEXPR_SINCE_CXX20
#    define OPENGEMINI_CONSTEVAL_SINCE_CXX20
#endif // !(__cplusplus >= 202002L)

#endif // !OPENGEMINI_UTIL_PREPROCESSOR_HPP
