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

#ifndef OPENGEMINI_UTIL_PREPROCESSOR_HPP
#define OPENGEMINI_UTIL_PREPROCESSOR_HPP

#ifdef OPENGEMINI_SEPERATE_COMPILATION
#    define OPENGEMINI_INLINE_SPECIFIER
#else
#    define OPENGEMINI_INLINE_SPECIFIER inline
#endif // OPENGEMINI_SEPERATE_COMPILATION

#endif // !OPENGEMINI_UTIL_PREPROCESSOR_HPP
