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

#ifndef OPENGEMINI_IMPL_UTIL_BASE64_HPP
#define OPENGEMINI_IMPL_UTIL_BASE64_HPP

#include <string>
#include <string_view>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace opengemini::util {

inline std::string Base64Encode(std::string_view data)
{
    using namespace boost::archive::iterators;
    using It = base64_from_binary<
        transform_width<std::string_view::const_iterator, 6, 8>>;
    auto res = std::string(It(std::cbegin(data)), It(std::cend(data)));
    return res.append((3 - data.size() % 3) % 3, '=');
}

} // namespace opengemini::util

#endif // !OPENGEMINI_IMPL_UTIL_BASE64_HPP
