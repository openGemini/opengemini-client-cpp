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

#include <future>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "opengemini/impl/comm/Context.hpp"
#include "opengemini/impl/http/HttpClient.hpp"
#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
#    include "opengemini/impl/http/HttpsClient.hpp"
#    include "test/SelfRootCA.hpp"
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT
#include "opengemini/Exception.hpp"
#include "test/Random.hpp"
#include "test/TestFixtureWithContext.hpp"

namespace opengemini::test {

using namespace std::chrono_literals;
using namespace impl::http;

class IHttpClientTestFixture : public TestFixtureWithContext {
protected:
    IHttpClientTestFixture()
    {
        clients_.emplace_back(std::make_unique<HttpClient>(ctx_(), 5s, 5s),
                              Endpoint{ "httpbin.org", 80 });
#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
        clients_.emplace_back(
            std::make_unique<HttpsClient>(ctx_(), 5s, 5s, TLSConfig{}),
            Endpoint{ "httpbin.org", 443 });
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT
    }

    template<typename... Args>
    Response DoGet(const std::unique_ptr<IHttpClient>& client, Args&&... args)
    {
        return boost::asio::spawn(
                   ctx_(),
                   [&](auto yield) {
                       return client->Get(std::forward<Args>(args)..., yield);
                   },
                   boost::asio::use_future)
            .get();
    }

    template<typename... Args>
    Response DoPost(const std::unique_ptr<IHttpClient>& client, Args&&... args)
    {
        return boost::asio::spawn(
                   ctx_(),
                   [&](auto yield) {
                       return client->Post(std::forward<Args>(args)..., yield);
                   },
                   boost::asio::use_future)
            .get();
    }

protected:
    std::vector<std::pair<std::unique_ptr<IHttpClient>, Endpoint>> clients_;
};

TEST_F(IHttpClientTestFixture, GetRequest)
{
    for (auto& [client, endpoint] : clients_) {
        auto rsp = DoGet(client, endpoint, "/get");
        EXPECT_EQ(rsp.result_int(), 200);
        EXPECT_TRUE(!rsp.body().empty());
    }
}

TEST_F(IHttpClientTestFixture, PostRequest)
{
    const auto body = test::GenerateRandomString(GenerateRandomNumber(1, 64));
    for (auto& [client, endpoint] : clients_) {
        auto rsp = DoPost(client, endpoint, "/anything", body);
        EXPECT_EQ(rsp.result_int(), 200);
        EXPECT_THAT(rsp.body(), testing::HasSubstr(body));
    }
}

TEST_F(IHttpClientTestFixture, CallFromMultiThreads)
{
    std::vector<std::future<void>> futures;
    for (auto idx = 0; idx < clients_.size(); ++idx) {
        for (auto cnt = 0; cnt < 10; ++cnt) {
            futures.push_back(std::async(std::launch::async, [this, idx] {
                auto rsp = DoGet(clients_[idx].first,
                                 clients_[idx].second,
                                 "/range/32");
                EXPECT_EQ(rsp.result_int(), 200);
                EXPECT_EQ(rsp.body(), "abcdefghijklmnopqrstuvwxyzabcdef");
            }));
        }
    }

    for (auto& future : futures) { future.get(); }
}

TEST_F(IHttpClientTestFixture, InvalidHost)
{
    for (auto& [client, _] : clients_) {
        EXPECT_THROW(DoGet(client, Endpoint{ "123.456.789.10", 123 }, "/get"),
                     Exception);
    }
}

TEST_F(IHttpClientTestFixture, InvalidPort)
{
    for (auto& [client, _] : clients_) {
        EXPECT_THROW(std::ignore =
                         DoGet(client, Endpoint{ "httpbin.org", 12345 }, "/"),
                     Exception);
    }
}

TEST_F(IHttpClientTestFixture, WithoutSetDefaultHeaders)
{
    using boost::beast::http::field;

    for (auto& [client, endpoint] : clients_) {
        auto rsp = DoGet(client, endpoint, "/headers");
        EXPECT_THAT(rsp.body(), testing::HasSubstr(R"("Host": "httpbin.org")"));
        EXPECT_THAT(rsp.body(),
                    testing::ContainsRegex(
                        R"(opengemini-client-cxx/[0-9]+\.[0-9]+\.[0-9]+)"));
    }
}

TEST_F(IHttpClientTestFixture, SetDefaultHeaders)
{
    using boost::beast::http::field;

    for (auto& [client, endpoint] : clients_) {
        client->DefaultHeaders() = { { "Content-Type", "text/plain" },
                                     { "Authorization", "dummy" } };

        auto rsp = DoGet(client, endpoint, "/headers");
        EXPECT_THAT(rsp.body(), testing::HasSubstr(R"("Host": "httpbin.org")"));
        EXPECT_THAT(
            rsp.body(),
            testing::ContainsRegex(
                R"("User-Agent": "opengemini-client-cxx/[0-9]+\.[0-9]+\.[0-9]+")"));
        EXPECT_THAT(rsp.body(),
                    testing::HasSubstr(R"("Content-Type": "text/plain")"));
        EXPECT_THAT(rsp.body(),
                    testing::HasSubstr(R"("Authorization": "dummy")"));
    }
}

TEST_F(IHttpClientTestFixture, SetDefaultHeadersWhichWillBeCovered)
{
    using boost::beast::http::field;

    for (auto& [client, endpoint] : clients_) {
        client->DefaultHeaders() = { { "User-Agent", "dummy" },
                                     { "Host", "dummy" } };

        auto rsp = DoGet(client, endpoint, "/headers");
        EXPECT_THAT(rsp.body(), testing::HasSubstr(R"("Host": "httpbin.org")"));
        EXPECT_THAT(
            rsp.body(),
            testing::ContainsRegex(
                R"("User-Agent": "opengemini-client-cxx/[0-9]+\.[0-9]+\.[0-9]+")"));
    }
}

TEST_F(IHttpClientTestFixture, ReadWriteTimeout)
{
    for (auto& [client, endpoint] : clients_) {
        EXPECT_THROW(std::ignore = DoGet(client, endpoint, "/delay/7"),
                     Exception);
    }
}

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT

TEST_F(IHttpClientTestFixture, WithInvalidRootCA)
{
    EXPECT_THROW(HttpsClient cli(ctx_(), 5s, 5s, { false, {}, "dummy ca" }),
                 Exception);
}

TEST_F(IHttpClientTestFixture, WithInvalidCertificate)
{
    EXPECT_THROW(
        HttpsClient cli(ctx_(), 5s, 5s, { false, "dummy", selfRootCA }),
        Exception);
}

TEST_F(IHttpClientTestFixture, WithSelfSignedRootCA)
{
    std::unique_ptr<IHttpClient> client =
        std::make_unique<HttpsClient>(ctx_(),
                                      5s,
                                      5s,
                                      TLSConfig{ false, {}, selfRootCA });

    EXPECT_THROW(std::ignore = DoGet(client, clients_[1].second, "/range/26"),
                 Exception);
}

TEST_F(IHttpClientTestFixture, SkipVerifyPeer)
{
    std::unique_ptr<IHttpClient> client =
        std::make_unique<HttpsClient>(ctx_(),
                                      5s,
                                      5s,
                                      TLSConfig{ true, {}, selfRootCA });

    auto rsp = DoGet(client, clients_[1].second, "/range/26");
    EXPECT_EQ(rsp.body(), "abcdefghijklmnopqrstuvwxyz");
}

#endif // OPENGEMINI_ENABLE_SSL_SUPPORT

} // namespace opengemini::test
