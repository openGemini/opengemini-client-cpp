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

using namespace std::chrono_literals;

namespace opengemini::test {

TEST(ClientConfigBuilderTest, BasicUsage)
{
    auto conf =
        ClientConfigBuilder{}
            .AppendAddress({ "127.0.0.1", 8086 })
            .AppendAddresses({ { "localhost", 1234 }, { "dummy-host", 18086 } })
            .EnableGzip(true)
            .AuthCredential("dummyuser", "dummypass")
            .ReadWriteTimeout(3500ms)
            .ConnectTimeout(20s)
            .BatchConfig(1min, 10000)
            .ConcurrencyHint(12)
            .Finalize();

    auto endpointPred = [](const Endpoint&  endpoint,
                           std::string_view expectedHost,
                           uint16_t         expectedPort) {
        return endpoint.host == expectedHost && endpoint.port == expectedPort;
    };
    EXPECT_PRED3(endpointPred, conf.addresses[0], "127.0.0.1", 8086);
    EXPECT_PRED3(endpointPred, conf.addresses[1], "localhost", 1234);
    EXPECT_PRED3(endpointPred, conf.addresses[2], "dummy-host", 18086);

    EXPECT_EQ(conf.gzipEnabled, true);
    EXPECT_EQ(conf.concurrencyHint, 12);

    const auto& [username, password] =
        std::get<AuthCredential>(conf.authConfig.value());
    EXPECT_EQ(username, "dummyuser");
    EXPECT_EQ(password, "dummypass");

    EXPECT_EQ(conf.timeout, 3500ms);
    EXPECT_EQ(conf.connectTimeout, 20s);

    EXPECT_EQ(conf.batchConfig->batchSize, 10000);
    EXPECT_EQ(conf.batchConfig->batchInterval, 1min);
}

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
TEST(ClientConfigBuilderTest, TLSConfig)
{
    auto conf = ClientConfigBuilder{}
                    .SkipVerifyPeer(true)
                    .RootCAs("dummy CAs")
                    .TLSVersion(TLSVersion::tlsv13)
                    .ClientCertificates("dummy certs")
                    .EnableTLS(false)
                    .Finalize();

    EXPECT_TRUE(conf.addresses.empty());
    EXPECT_TRUE(conf.tlsConfig->skipVerifyPeer);
    EXPECT_EQ(conf.tlsConfig->rootCAs, "dummy CAs");
    EXPECT_EQ(conf.tlsConfig->certificates, "dummy certs");
    EXPECT_EQ(conf.tlsConfig->version, TLSVersion::tlsv13);
    EXPECT_FALSE(conf.tlsEnabled);
}
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT

} // namespace opengemini::test
