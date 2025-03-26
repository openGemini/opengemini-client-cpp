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

#include "opengemini/ClientConfigBuilder.hpp"

#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini {

OPENGEMINI_INLINE_SPECIFIER
const ClientConfig& ClientConfigBuilder::Finalize() const noexcept
{
    return conf_;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::AppendAddress(const Endpoint& address)
{
    conf_.addresses.push_back(address);
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder&
ClientConfigBuilder::AppendAddresses(const std::vector<Endpoint>& addresses)
{
    auto& addrs_ = conf_.addresses;
    addrs_.insert(addrs_.end(), addresses.begin(), addresses.end());
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder&
ClientConfigBuilder::AuthCredential(const std::string& username,
                                    const std::string& password)
{
    struct AuthCredential cred{ username, password };
    conf_.authConfig.emplace(std::move(cred));
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::AuthToken(const std::string& token)
{
    conf_.authConfig.emplace(token);
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder&
ClientConfigBuilder::BatchConfig(std::chrono::milliseconds interval,
                                 std::size_t               size)
{
    struct BatchConfig batch{ interval, size };
    conf_.batchConfig.emplace(std::move(batch));
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder&
ClientConfigBuilder::ReadWriteTimeout(std::chrono::milliseconds timeout)
{
    conf_.timeout = timeout;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder&
ClientConfigBuilder::ConnectTimeout(std::chrono::milliseconds timeout)
{
    conf_.connectTimeout = timeout;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::EnableGzip(bool enabled)
{
    conf_.gzipEnabled = enabled;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::ConcurrencyHint(std::size_t hint)
{
    conf_.concurrencyHint = hint;
    return *this;
}

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::SkipVerifyPeer(bool skipped)
{
    PrepareTLSConfig().skipVerifyPeer = skipped;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder&
ClientConfigBuilder::ClientCertificates(const std::string& certificates)
{
    PrepareTLSConfig().certificates = certificates;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::RootCAs(const std::string& rootCAs)
{
    PrepareTLSConfig().rootCAs = rootCAs;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::TLSVersion(enum TLSVersion version)
{
    PrepareTLSConfig().version = version;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
ClientConfigBuilder& ClientConfigBuilder::EnableTLS(bool enabled)
{
    conf_.tlsEnabled = enabled;
    return *this;
}

OPENGEMINI_INLINE_SPECIFIER
TLSConfig& ClientConfigBuilder::PrepareTLSConfig()
{
    auto& tls = conf_.tlsConfig;
    if (!tls.has_value()) { tls.emplace(TLSConfig{}); }
    return tls.value();
}
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT

} // namespace opengemini
