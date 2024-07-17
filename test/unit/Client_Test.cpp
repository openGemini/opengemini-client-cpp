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

// Note: Tests in the ClientTest suite need to be run in a real environment with
// OpenGemini deployed, which should listen on 127.0.0.1:8086.

#include <gtest/gtest.h>

#include "opengemini/Client.hpp"
#include "opengemini/ClientConfigBuilder.hpp"

using namespace std::chrono_literals;

namespace opengemini::test {

TEST(ClientTest, Ping)
{
    Client client{
        ClientConfigBuilder().AppendAddress({ "127.0.0.1", 8086 }).Finalize()
    };

    auto version = client.Ping(0);
    EXPECT_FALSE(version.empty());
}

TEST(ClientTest, Query)
{
    Client client{
        ClientConfigBuilder().AppendAddress({ "127.0.0.1", 8086 }).Finalize()
    };

    auto queryResult = client.Query({
        "ExampleDatabase",
        "select * from ExampleMeasurement",
    });

    std::cout << queryResult << std::endl;
}

} // namespace opengemini::test
