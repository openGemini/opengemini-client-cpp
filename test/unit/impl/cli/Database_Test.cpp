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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "opengemini/CompletionToken.hpp"
#include "test/ClientImplTestFixture.hpp"
#include "test/ExpectThrowAs.hpp"

namespace opengemini::test {

using namespace std::string_literals;
using namespace duration_literals;

class DatabaseTestFixture : public test::ClientImplTestFixture { };

MATCHER_P(IsTargetEq,
          expect,
          "Target"s + (negation ? "is" : "isn't") + " equal to " +
              testing::PrintToString(expect))
{
    std::cout << "Actual target: " << arg.target() << std::endl;
    return arg.target() == expect;
}

#define EXPECT_TARGET(TARGET)                                                  \
    EXPECT_CALL(                                                               \
        *mockHttp_,                                                            \
        SendRequest(testing::Field(&Endpoint::host, testing::Eq("127.0.0.1")), \
                    IsTargetEq(TARGET),                                        \
                    testing::_))                                               \
        .Times(1)                                                              \
        .WillOnce(                                                             \
            testing::Return(http::Response{ http::Status::ok, 11, "{}" }))

TEST_F(DatabaseTestFixture, CreateDatabaseSuccess)
{
    EXPECT_TARGET(R"(/query?db=&q=CREATE%20DATABASE%20%22test_db_name%22)");

    EXPECT_NO_THROW(impl_.CreateDatabase("test_db_name", {}, token::sync));
}

TEST_F(DatabaseTestFixture, CreateDatabaseWithEmptyName)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS(impl_.CreateDatabase("", {}, token::sync),
                    errc::LogicErrors::InvalidArgument);
    EXPECT_THROW_AS(impl_.CreateDatabase({}, {}, token::sync),
                    errc::LogicErrors::InvalidArgument);
}

TEST_F(DatabaseTestFixture, CreateDatabaseWithRpAllEmpty)
{
    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20DATABASE%20%22test_db_name%22%20WITH%20REPLICATION%201)");

    EXPECT_NO_THROW(
        impl_.CreateDatabase("test_db_name", RpConfig{}, token::sync));
}

TEST_F(DatabaseTestFixture, CreateDatabaseWithRpDuration)
{
    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20DATABASE%20%22test_db_name%22%20WITH%20DURATION%203d%20REPLICATION%201)");

    EXPECT_NO_THROW(impl_.CreateDatabase("test_db_name",
                                         RpConfig{ {}, 3_day },
                                         token::sync));
}

TEST_F(DatabaseTestFixture, CreateDatabaseWithRpShardDuration)
{
    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20DATABASE%20%22test_db_name%22%20WITH%20REPLICATION%201%20SHARD%20DURATION%204h)");

    EXPECT_NO_THROW(impl_.CreateDatabase("test_db_name",
                                         RpConfig{ {}, {}, 4_hour },
                                         token::sync));
}

TEST_F(DatabaseTestFixture, CreateDatabaseWithRpIndexDuration)
{
    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20DATABASE%20%22test_db_name%22%20WITH%20REPLICATION%201%20INDEX%20DURATION%205w)");

    EXPECT_NO_THROW(impl_.CreateDatabase("test_db_name",
                                         RpConfig{ {}, {}, {}, 5_week },
                                         token::sync));
}

TEST_F(DatabaseTestFixture, CreateDatabaseWithRpPolicyName)
{
    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20DATABASE%20%22test_db_name%22%20WITH%20REPLICATION%201%20NAME%20test_policy_name)");

    EXPECT_NO_THROW(impl_.CreateDatabase("test_db_name",
                                         RpConfig{ "test_policy_name" },
                                         token::sync));
}

TEST_F(DatabaseTestFixture, CreateDatabaseWithRpAll)
{
    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20DATABASE%20%22test_db_name%22%20WITH%20DURATION%203w%20REPLICATION%201%20SHARD%20DURATION%202d%20INDEX%20DURATION%201h%20NAME%20test_policy_name)");

    EXPECT_NO_THROW(impl_.CreateDatabase(
        "test_db_name",
        RpConfig{ "test_policy_name", 3_week, 2_day, 1_hour },
        token::sync));
}

TEST_F(DatabaseTestFixture, CreateDatabaseError)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(1)
        .WillOnce(
            testing::Return(http::Response{ http::Status::ok,
                                            11,
                                            R"({"error":"dummy error"})" }));

    EXPECT_THROW_AS(impl_.CreateDatabase("test_db_name", {}, token::sync),
                    errc::ServerErrors::ErrorResult);
}

TEST_F(DatabaseTestFixture, ShowDatabaseSuccess)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(3)
        .WillOnce(testing::Return(http::Response{
            http::Status::ok,
            11,
            R"({"results":[{"series":[{"values":[["db1",0.1],[1234,"str"],["db2",true],["db3"],["db4"]]}]}]})" }))
        .WillOnce(testing::Return(http::Response{ http::Status::ok, 11, "{}" }))
        .WillOnce(testing::Return(http::Response{
            http::Status::ok,
            11,
            R"({"results":[{"series":[{"values":[[true,0.1],[1234,"str"],[0.345,true],[]]}]}]})" }));

    EXPECT_EQ(impl_.ShowDatabase(token::sync),
              (std::vector<std::string>{ "db1", "db2", "db3", "db4" }));
    EXPECT_TRUE(impl_.ShowDatabase(token::sync).empty());
    EXPECT_TRUE(impl_.ShowDatabase(token::sync).empty());
}

TEST_F(DatabaseTestFixture, DropDatabaseSuccess)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(1)
        .WillOnce(
            testing::Return(http::Response{ http::Status::ok, 11, "{}" }));

    EXPECT_NO_THROW(impl_.DropDatabase("test_db_name", token::sync));
}

TEST_F(DatabaseTestFixture, DropDatabaseWithEmptyName)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS(impl_.DropDatabase({}, token::sync),
                    errc::LogicErrors::InvalidArgument);
    EXPECT_THROW_AS(impl_.DropDatabase("", token::sync),
                    errc::LogicErrors::InvalidArgument);
}

} // namespace opengemini::test
