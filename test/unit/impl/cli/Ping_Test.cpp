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

#include <gtest/gtest.h>

#include "opengemini/ClientConfigBuilder.hpp"
#include "opengemini/CompletionToken.hpp"
#include "opengemini/impl/ClientImpl.hpp"
#include "test/ExpectThrowAs.hpp"
#include "test/HackMember.hpp"
#include "test/MockIHttpClient.hpp"

namespace opengemini::test {

using namespace opengemini::impl;

OPENGEMINI_TEST_MEMBER_HACKER(ClientImpl,
                              &ClientImpl::ctx_,  // 0
                              &ClientImpl::http_, // 1
                              &ClientImpl::lb_)   // 2

class PingTestFixture : public testing::Test {
protected:
    PingTestFixture() :
        impl_(ClientConfigBuilder()
                  .AppendAddress({ "127.0.0.1", 1234 })
                  .AppendAddress({ "127.0.0.1", 4321 })
                  .Finalize())
    {
        auto  hackImpl = HackingMember(impl_);
        auto& ctx_     = impl_.*(std::get<0>(hackImpl));
        mockHttp_      = std::make_shared<MockIHttpClient>(ctx_());
        impl_.*(std::get<1>(hackImpl)) = mockHttp_;
    }

protected:
    ClientImpl                       impl_;
    std::shared_ptr<MockIHttpClient> mockHttp_;
};

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
