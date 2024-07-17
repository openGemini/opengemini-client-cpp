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

class QueryTestFixture : public test::ClientImplTestFixture { };

MATCHER_P(IsQueryTargetEq,
          expect,
          "Query target "s + (negation ? "is" : "isn't") + " equal to " +
              testing::PrintToString(expect))
{
    return arg.target() == expect;
}

TEST_F(QueryTestFixture, Success)
{
    EXPECT_CALL(
        *mockHttp_,
        SendRequest(testing::Field(&Endpoint::host, testing::Eq("127.0.0.1")),
                    IsQueryTargetEq("/query?db=db&q=command&rp=&epoch=ns"),
                    testing::_))
        .Times(1)
        .WillRepeatedly(
            testing::Return(http::Response{ http::Status::ok, 11, "{}" }));

    EXPECT_NO_THROW(impl_.Query({ "db", "command" }, token::sync));
}

TEST_F(QueryTestFixture, EmptyCommand)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS((std::ignore = impl_.Query({ "db", {} }, token::sync)),
                    errc::LogicErrors::InvalidArgument);
}

} // namespace opengemini::test
