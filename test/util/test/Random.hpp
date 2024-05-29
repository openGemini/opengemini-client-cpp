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

#ifndef TEST_UTIL_TEST_RANDOM_HPP
#define TEST_UTIL_TEST_RANDOM_HPP

#include <random>
#include <string>

namespace opengemini::test {

inline int GenerateRandomNumber(int min, int max)
{
    std::random_device            seed;
    std::mt19937                  engine(seed());
    std::uniform_int_distribution generator(min, max);
    return generator(engine);
}

inline std::string GenerateRandomString(std::size_t length)
{
    std::string dict{
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    };
    std::random_device              seed;
    std::mt19937                    engine(seed());
    std::uniform_int_distribution<> generator(0, dict.size() - 1);

    std::string result;
    for (int cnt = 0; cnt < length; ++cnt) {
        result.push_back(dict[generator(engine)]);
    }
    return result;
}

} // namespace opengemini::test

#endif // !TEST_UTIL_TEST_RANDOM_HPP
