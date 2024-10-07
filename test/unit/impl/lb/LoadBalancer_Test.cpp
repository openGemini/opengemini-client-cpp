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

#include <future>
#include <unordered_set>

#include <gtest/gtest.h>

#include "opengemini/impl/lb/LoadBalancer.hpp"
#include "test/ExpectThrowAs.hpp"
#include "test/MockIHttpClient.hpp"
#include "test/Random.hpp"
#include "test/TestFixtureWithContext.hpp"

using namespace std::chrono_literals;

namespace opengemini::test {

using namespace opengemini::impl;

class LoadBalancerTestFixture : public TestFixtureWithContext {
protected:
    LoadBalancerTestFixture() :
        endpoints_([] {
            std::vector<Endpoint> endpoints(GenerateRandomNumber(2, 50));
            std::generate(endpoints.begin(), endpoints.end(), [] {
                return Endpoint{ GenerateRandomString(10),
                                 static_cast<uint16_t>(
                                     GenerateRandomNumber(1, 65535)) };
            });
            return endpoints;
        }())
    { }

protected:
    std::vector<Endpoint> endpoints_;
};

TEST_F(LoadBalancerTestFixture, ConstructWithInvalidEndpoint)
{
    EXPECT_THROW_AS(
        (std::ignore = lb::LoadBalancer::Construct(ctx_(),
                                                   std::vector<Endpoint>{},
                                                   nullptr)),
        errc::LogicErrors::InvalidArgument)

    EXPECT_THROW_AS(
        (std::ignore = lb::LoadBalancer::Construct(
             ctx_(),
             std::vector<Endpoint>{ { "host1", 1 }, { "", 2 }, { "host3", 3 } },
             nullptr)),
        errc::LogicErrors::InvalidArgument);

    EXPECT_THROW_AS((std::ignore = lb::LoadBalancer::Construct(
                         ctx_(),
                         std::vector<Endpoint>{ { "host1", 1 },
                                                { "host2", 2 },
                                                { "host3", 3 },
                                                { "host2", 4 },
                                                { "host2", 2 } },
                         nullptr)),
                    errc::LogicErrors::InvalidArgument);
}

TEST_F(LoadBalancerTestFixture, PickSpecificServer)
{
    auto lb = lb::LoadBalancer::Construct(ctx_(), endpoints_, nullptr);
    for (std::size_t idx = 0; idx < endpoints_.size(); ++idx) {
        EXPECT_EQ(lb->PickServer(idx), endpoints_[idx]);
    }

    EXPECT_THROW_AS(lb->PickServer(endpoints_.size()),
                    errc::LogicErrors::InvalidArgument);
}

TEST_F(LoadBalancerTestFixture, PickAllServers)
{
    auto lb = lb::LoadBalancer::Construct(ctx_(), endpoints_, nullptr);
    for (std::size_t idx = 0; idx < endpoints_.size(); ++idx) {
        EXPECT_EQ(lb->PickAvailableServer(), endpoints_[idx]);
    }
}

TEST_F(LoadBalancerTestFixture, HealthCheckAllAvailable)
{
    auto mockHttp = std::make_shared<MockIHttpClient>(ctx_());
    EXPECT_CALL(*mockHttp, SendRequest(testing::_, testing::_, testing::_))
        .Times(testing::AtLeast(endpoints_.size()))
        .WillRepeatedly(
            testing::Return(http::Response{ http::Status::no_content, 11 }));

    auto lb = lb::LoadBalancer::Construct(ctx_(), endpoints_, mockHttp, 100ms);
    lb->StartHealthCheck();
    std::this_thread::sleep_for(500ms);

    for (const auto& endpoint : endpoints_) {
        EXPECT_EQ(lb->PickAvailableServer(), endpoint);
    }
}

TEST_F(LoadBalancerTestFixture, HealthCheckAllUnavailable)
{
    auto mockHttp = std::make_shared<MockIHttpClient>(ctx_());
    EXPECT_CALL(*mockHttp, SendRequest(testing::_, testing::_, testing::_))
        .Times(testing::AtLeast(endpoints_.size()))
        .WillRepeatedly(testing::Throw(std::runtime_error("dummy error")));

    auto lb = lb::LoadBalancer::Construct(ctx_(), endpoints_, mockHttp, 100ms);
    lb->StartHealthCheck();
    std::this_thread::sleep_for(500ms);

    EXPECT_THROW_AS(lb->PickAvailableServer(),
                    errc::ServerErrors::NoAvailableServer);
}

TEST_F(LoadBalancerTestFixture, HealthCheckPartiallyAvailable)
{
    std::unordered_set<Endpoint, Endpoint::Hasher> unavailableEndpoints;
    std::generate_n(
        std::inserter(unavailableEndpoints, unavailableEndpoints.begin()),
        GenerateRandomNumber(1, endpoints_.size() / 2),
        [this] {
            return endpoints_[GenerateRandomNumber(0, endpoints_.size() - 1)];
        });

    auto mockHttp = std::make_shared<MockIHttpClient>(ctx_());
    EXPECT_CALL(
        *mockHttp,
        SendRequest(testing::Not(testing::AnyOfArray(unavailableEndpoints)),
                    testing::_,
                    testing::_))
        .Times(
            testing::AtLeast(endpoints_.size() - unavailableEndpoints.size()))
        .WillRepeatedly(
            testing::Return(http::Response{ http::Status::no_content, 11 }));
    EXPECT_CALL(*mockHttp,
                SendRequest(testing::AnyOfArray(unavailableEndpoints),
                            testing::_,
                            testing::_))
        .Times(testing::AtLeast(unavailableEndpoints.size()))
        .WillRepeatedly(testing::Throw(std::runtime_error("dummy error")));

    auto lb = lb::LoadBalancer::Construct(ctx_(), endpoints_, mockHttp, 100ms);
    lb->StartHealthCheck();
    std::this_thread::sleep_for(500ms);

    std::vector<std::future<Endpoint>> futures;
    for (std::size_t cnt = 0; cnt < endpoints_.size() * 2; ++cnt) {
        futures.emplace_back(
            std::async([&lb] { return lb->PickAvailableServer(); }));
    }
    for (auto& future : futures) {
        EXPECT_THAT(future.get(),
                    testing::Not(testing::AnyOfArray(unavailableEndpoints)));
    }
}

} // namespace opengemini::test
