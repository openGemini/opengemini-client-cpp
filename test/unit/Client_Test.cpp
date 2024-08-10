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

#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "opengemini/Client.hpp"
#include "opengemini/ClientConfigBuilder.hpp"
#include "opengemini/RetentionPolicy.hpp"

using namespace std::chrono_literals;

namespace opengemini::test {

// Note: Tests in the ClientTest suite need to be run in a real environment with
// OpenGemini deployed, which should listen on 127.0.0.1:8086.

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

TEST(ClientTest, Database)
{
    using namespace duration_literals;

    Client client{
        ClientConfigBuilder().AppendAddress({ "127.0.0.1", 8086 }).Finalize()
    };

    constexpr auto db1{ "test_db_cxx1" };
    constexpr auto db2{ "test_db_cxx2" };

    client.CreateDatabase(db1, RpConfig{ "rp", 5_day, 2_hour, 1_week });
    client.CreateDatabase(db2);
    std::this_thread::sleep_for(500ms);

    auto dbs = client.ShowDatabase();
    EXPECT_THAT(dbs,
                testing::AllOf(testing::Contains(db1), testing::Contains(db2)));

    client.DropDatabase(db1);
    std::this_thread::sleep_for(500ms);

    dbs = client.ShowDatabase();
    EXPECT_THAT(dbs,
                testing::AllOf(testing::Not(testing::Contains(db1)),
                               testing::Contains(db2)));

    client.DropDatabase(db2);
    std::this_thread::sleep_for(500ms);

    dbs = client.ShowDatabase();
    EXPECT_THAT(dbs,
                testing::Not(testing::AllOf(testing::Contains(db1),
                                            testing::Contains(db2))));
}

TEST(ClientTest, RetentionPolicy)
{
    using namespace duration_literals;

    Client client{
        ClientConfigBuilder().AppendAddress({ "127.0.0.1", 8086 }).Finalize()
    };

    constexpr auto db{ "test_rp_cxx" };
    constexpr auto rp1{ "rp1" };
    constexpr auto rp2{ "rp2" };
    constexpr auto rp3{ "rp3" };

    client.CreateDatabase(db, RpConfig{ rp1, 1_week, 2_day, 3_hour });
    client.CreateRetentionPolicy(db, { rp2, 4_day, 5_hour, 6_minute }, true);
    client.CreateRetentionPolicy(db, { rp3, 7_day, 8_hour, 9_hour }, false);
    std::this_thread::sleep_for(500ms);

    auto rps = client.ShowRetentionPolicies(db);
    EXPECT_EQ(rps.size(), 3);

    client.DropRetentionPolicy(db, rp1);
    std::this_thread::sleep_for(500ms);

    rps = client.ShowRetentionPolicies(db);
    EXPECT_EQ(rps.size(), 2);

    client.DropDatabase(db);
}

} // namespace opengemini::test
