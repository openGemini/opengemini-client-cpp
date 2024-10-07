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

#include <gtest/gtest.h>

#include "opengemini/CompletionToken.hpp"
#include "test/ClientImplTestFixture.hpp"
#include "test/ExpectThrowAs.hpp"

namespace opengemini::test {

class PingTestFixture : public test::ClientImplTestFixture { };

TEST_F(PingTestFixture, Success)
{
    EXPECT_CALL(*mockHttp_,
                SendRequest(testing::Eq(Endpoint{ "127.0.0.1", 1234 }),
                            testing::_,
                            testing::_))
        .Times(1)
        .WillOnce(
            testing::Return(http::Response{ http::Status::no_content, 11 }));

    EXPECT_CALL(*mockHttp_,
                SendRequest(testing::Eq(Endpoint{ "127.0.0.1", 4321 }),
                            testing::_,
                            testing::_))
        .Times(1)
        .WillOnce(testing::Return([]() {
            auto rsp = http::Response{ http::Status::no_content, 11 };
            rsp.insert("X-Geminidb-Version", "dummy_version");
            return rsp;
        }()));

    EXPECT_EQ(impl_.Ping(0, token::sync), "Unknown");
    EXPECT_EQ(impl_.Ping(1, token::sync), "dummy_version");
}

TEST_F(PingTestFixture, InvalidIndex)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(0);

    EXPECT_THROW_AS(impl_.Ping(2, token::sync),
                    errc::LogicErrors::InvalidArgument);
}

TEST_F(PingTestFixture, UnexpectedErrorOccur)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(1)
        .WillOnce(testing::Throw(std::runtime_error("dummy error")));

    EXPECT_THROW_AS(impl_.Ping(0, token::sync),
                    errc::RuntimeErrors::Unexpected);
}

TEST_F(PingTestFixture, ReceiveUnexpectedStatusCodeFromServer)
{
    EXPECT_CALL(*mockHttp_, SendRequest(testing::_, testing::_, testing::_))
        .Times(1)
        .WillOnce(testing::Return(http::Response{ http::Status::ok, 11 }));

    EXPECT_THROW_AS(impl_.Ping(0, token::sync),
                    errc::ServerErrors::UnexpectedStatusCode);
}

} // namespace opengemini::test
