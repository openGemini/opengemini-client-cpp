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

class WriteTestFixture : public test::ClientImplTestFixture { };

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
        .WillOnce(testing::Return(                                             \
            http::Response{ http::Status::no_content, 11, "{}" }))

TEST_F(WriteTestFixture, SingleWriteSuccess)
{
    EXPECT_TARGET(R"(/write?db=test_db_cxx&rp=)");
    impl_.Write<Point>(
        "test_db_cxx",
        { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } },
        {},
        token::sync);

    EXPECT_TARGET(R"(/write?db=test_db_cxx&rp=test_rp_cxx)");
    impl_.Write<Point>(
        "test_db_cxx",
        { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } },
        "test_rp_cxx",
        token::sync);
}

TEST_F(WriteTestFixture, SingleWriteWithInvalidEmptyDatabaseName)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);
    EXPECT_THROW_AS(
        impl_.Write<Point>(
            {},
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } },
            "test_rp_cxx",
            token::sync),
        errc::LogicErrors::InvalidArgument);
}

TEST_F(WriteTestFixture, SingleWriteWithInvalidPoint)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS(
        impl_.Write<Point>("test_db_cxx", {}, "test_rp_cxx", token::sync),
        errc::LogicErrors::InvalidArgument);

    EXPECT_THROW_AS(impl_.Write<Point>("test_db_cxx",
                                       { "test_measurement", {} },
                                       "test_rp_cxx",
                                       token::sync),
                    errc::LogicErrors::InvalidArgument);

    EXPECT_THROW_AS(impl_.Write<Point>("test_db_cxx",
                                       { {}, { { "field1", 1 } } },
                                       "test_rp_cxx",
                                       token::sync),
                    errc::LogicErrors::InvalidArgument);
}

TEST_F(WriteTestFixture, MultipleWriteSuccess)
{
    std::vector<Point> points{
        { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } },
        { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } },
        { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } },
    };

    EXPECT_TARGET(R"(/write?db=test_db_cxx&rp=)");
    impl_.Write<std::vector<Point>>("test_db_cxx", points, {}, token::sync);

    EXPECT_TARGET(R"(/write?db=test_db_cxx&rp=test_rp_cxx)");
    impl_.Write<std::vector<Point>>("test_db_cxx",
                                    std::move(points),
                                    "test_rp_cxx",
                                    token::sync);
}

TEST_F(WriteTestFixture, MultipleWriteWithEmptyPointsVector)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_NO_THROW(
        impl_.Write<std::vector<Point>>("test_db_cxx", {}, {}, token::sync));
}

TEST_F(WriteTestFixture, MultipleWriteWithInvalidPoints)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS(
        impl_.Write<std::vector<Point>>("test_db_cxx",
                                        {
                                            { "test", { { "a", 1 } } },
                                            { {}, { { "a", 1 } } },
                                            { "test", { { "a", 1 } } },
                                        },
                                        "test_rp_cxx",
                                        token::sync),
        errc::LogicErrors::InvalidArgument);

    EXPECT_THROW_AS(
        impl_.Write<std::vector<Point>>("test_db_cxx",
                                        {
                                            { "test", { { "a", 1 } } },
                                            { "test", { { "a", 1 } } },
                                            { "test", {} },
                                        },
                                        "test_rp_cxx",
                                        token::sync),
        errc::LogicErrors::InvalidArgument);
}

} // namespace opengemini::test
