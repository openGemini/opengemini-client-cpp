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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "opengemini/CompletionToken.hpp"
#include "test/ClientImplTestFixture.hpp"
#include "test/ExpectThrowAs.hpp"

namespace opengemini::test {

using namespace std::string_literals;
using namespace duration_literals;

class RetentionPolicyTestFixture : public test::ClientImplTestFixture { };

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
        .WillOnce(                                                             \
            testing::Return(http::Response{ http::Status::ok, 11, "{}" }))

#define WILL_RETURN_RSP(RSP_CONTENT)                                         \
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_)) \
        .WillOnce(testing::Return(                                           \
            http::Response{ http::Status::ok, 11, RSP_CONTENT }))

TEST_F(RetentionPolicyTestFixture, CreateRetentionPolicySuccess)
{
    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22%20DURATION%203d%20REPLICATION%201)");
    EXPECT_NO_THROW(impl_.CreateRetentionPolicy("test_db_name",
                                                { "test_rp_name", 3_day },
                                                false,
                                                token::sync));

    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22%20DURATION%203d%20REPLICATION%201%20DEFAULT)");
    EXPECT_NO_THROW(impl_.CreateRetentionPolicy("test_db_name",
                                                { "test_rp_name", 3_day },
                                                true,
                                                token::sync));

    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22%20DURATION%203d%20REPLICATION%201%20SHARD%20DURATION%202h)");
    EXPECT_NO_THROW(
        impl_.CreateRetentionPolicy("test_db_name",
                                    { "test_rp_name", 3_day, 2_hour },
                                    false,
                                    token::sync));

    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22%20DURATION%203w%20REPLICATION%201%20INDEX%20DURATION%201h)");
    EXPECT_NO_THROW(
        impl_.CreateRetentionPolicy("test_db_name",
                                    { "test_rp_name", 3_week, {}, 1_hour },
                                    false,
                                    token::sync));

    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22%20DURATION%203w%20REPLICATION%201%20INDEX%20DURATION%201h)");
    EXPECT_NO_THROW(
        impl_.CreateRetentionPolicy("test_db_name",
                                    { "test_rp_name", 3_week, {}, 1_hour },
                                    false,
                                    token::sync));

    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22%20DURATION%205w%20REPLICATION%201%20SHARD%20DURATION%202d%20INDEX%20DURATION%201h%20DEFAULT)");
    EXPECT_NO_THROW(
        impl_.CreateRetentionPolicy("test_db_name",
                                    { "test_rp_name", 5_week, 2_day, 1_hour },
                                    true,
                                    token::sync));

    EXPECT_TARGET(
        R"(/query?db=&q=CREATE%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22%20DURATION%205w%20REPLICATION%201%20SHARD%20DURATION%202d%20INDEX%20DURATION%201h)");
    EXPECT_NO_THROW(
        impl_.CreateRetentionPolicy("test_db_name",
                                    { "test_rp_name", 5_week, 2_day, 1_hour },
                                    false,
                                    token::sync));
}

TEST_F(RetentionPolicyTestFixture, CreateRetentionPolicyWithInvalidArgument)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS(impl_.CreateRetentionPolicy("",
                                                { "test_rp_name", 3_day },
                                                false,
                                                token::sync),
                    errc::LogicErrors::InvalidArgument);

    EXPECT_THROW_AS(impl_.CreateRetentionPolicy("test_db_name",
                                                { "test_rp_name", "" },
                                                false,
                                                token::sync),
                    errc::LogicErrors::InvalidArgument);

    EXPECT_THROW_AS(impl_.CreateRetentionPolicy("test_db_name",
                                                { "", 3_day },
                                                false,
                                                token::sync),
                    errc::LogicErrors::InvalidArgument);
}

TEST_F(RetentionPolicyTestFixture, CreateRetentionPolicyError)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(1)
        .WillOnce(
            testing::Return(http::Response{ http::Status::ok,
                                            11,
                                            R"({"error":"dummy error"})" }));

    EXPECT_THROW_AS(impl_.CreateRetentionPolicy("test_db_name",
                                                { "test_rp_name", 3_day },
                                                false,
                                                token::sync),
                    errc::ServerErrors::ErrorResult);
}

TEST_F(RetentionPolicyTestFixture, ShowRetentionPoliciesSuccess)
{
    WILL_RETURN_RSP(
        R"({"results":[{"statement_id":0,"series":[{"columns":["name","duration","shardGroupDuration","hot duration","warm duration","index duration","replicaN","default"],"values":[["rp1","168h0m0s","48h0m0s","0s","0s","48h0m0s",1,false],["rp2","96h0m0s","5h0m0s","0s","0s","5h0m0s",1,true],["rp3","168h0m0s","8h0m0s","0s","0s","16h0m0s",1,false]]}]}]})");
    EXPECT_EQ(
        impl_.ShowRetentionPolicies("test_db_name", token::sync),
        (std::vector<RetentionPolicy>{
            { "rp1", "168h0m0s", "48h0m0s", "0s", "0s", "48h0m0s", 1, false },
            { "rp2", "96h0m0s", "5h0m0s", "0s", "0s", "5h0m0s", 1, true },
            { "rp3", "168h0m0s", "8h0m0s", "0s", "0s", "16h0m0s", 1, false },
        }));

    WILL_RETURN_RSP(
        R"({"results":[{"statement_id":0,"series":[{"columns":["name","duration","shardGroupDuration","hot duration","warm duration","index duration","replicaN","default"],"values":[["rp2","96h0m0s","5h0m0s","0s","0s","5h0m0s",1,true],["rp3","168h0m0s","8h0m0s","0s","0s","16h0m0s",1,false]]}]}]})");
    EXPECT_EQ(
        impl_.ShowRetentionPolicies("test_db_name", token::sync),
        (std::vector<RetentionPolicy>{
            { "rp2", "96h0m0s", "5h0m0s", "0s", "0s", "5h0m0s", 1, true },
            { "rp3", "168h0m0s", "8h0m0s", "0s", "0s", "16h0m0s", 1, false },
        }));

    WILL_RETURN_RSP(
        R"({"results":[{"statement_id":0,"series":[{"columns":["name","duration","shardGroupDuration","hot duration","warm duration","index duration","replicaN","default"],"values":[["rp1","168h0m0s","48h0m0s","0s","0s","48h0m0s","wrong_type",false],["rp2","96h0m0s","5h0m0s","0s","0s","5h0m0s",1,true],["rp3","168h0m0s","8h0m0s","0s","0s","16h0m0s",1,333]]}]}]})");
    EXPECT_EQ(impl_.ShowRetentionPolicies("test_db_name", token::sync),
              (std::vector<RetentionPolicy>{
                  { "rp2", "96h0m0s", "5h0m0s", "0s", "0s", "5h0m0s", 1, true },
              }));

    WILL_RETURN_RSP(R"({"results":[{"statement_id":1,"series":[]}]})");
    EXPECT_TRUE(
        impl_.ShowRetentionPolicies("test_db_name", token::sync).empty());

    WILL_RETURN_RSP(
        R"({"results":[{"statement_id":0,"series":[{"columns":["name","duration","shardGroupDuration","hot duration","warm duration","index duration","replicaN"],"values":[["rp2","96h0m0s","5h0m0s","0s","0s","5h0m0s",1],["rp3","168h0m0s","8h0m0s","0s","0s","16h0m0s",1]]}]}]})");
    EXPECT_TRUE(
        impl_.ShowRetentionPolicies("test_db_name", token::sync).empty());

    WILL_RETURN_RSP(R"({})");
    EXPECT_TRUE(
        impl_.ShowRetentionPolicies("test_db_name", token::sync).empty());
}

TEST_F(RetentionPolicyTestFixture, ShowRetentionPoliciesError)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(1)
        .WillOnce(
            testing::Return(http::Response{ http::Status::ok,
                                            11,
                                            R"({"error":"dummy error"})" }));

    EXPECT_THROW_AS(impl_.ShowRetentionPolicies("test_db_name", token::sync),
                    errc::ServerErrors::ErrorResult);
}

TEST_F(RetentionPolicyTestFixture, DropRetentionPolicySuccess)
{
    EXPECT_TARGET(
        R"(/query?db=&q=DROP%20RETENTION%20POLICY%20test_rp_name%20ON%20%22test_db_name%22)");

    EXPECT_NO_THROW(
        impl_.DropRetentionPolicy("test_db_name", "test_rp_name", token::sync));
}

TEST_F(RetentionPolicyTestFixture, DropRetentionPolicyWithInvalidArgument)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS(impl_.DropRetentionPolicy({}, "test_rp_name", token::sync),
                    errc::LogicErrors::InvalidArgument);

    EXPECT_THROW_AS(impl_.DropRetentionPolicy("test_db_name", {}, token::sync),
                    errc::LogicErrors::InvalidArgument);
}

} // namespace opengemini::test
