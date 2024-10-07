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

#include "opengemini/impl/ClientImpl.hpp"

#include <fmt/format.h>

#include "opengemini/Exception.hpp"
#include "opengemini/impl/http/HttpClient.hpp"
#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
#    include "opengemini/impl/http/HttpsClient.hpp"
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT
#include "opengemini/impl/util/Base64.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl {

OPENGEMINI_INLINE_SPECIFIER
ClientImpl::ClientImpl(const ClientConfig& config) :
    ctx_(config.concurrencyHint),
    http_(ConstructHttpClient(config)),
    lb_(lb::LoadBalancer::Construct(ctx_(), config.addresses, http_))
{
    lb_->StartHealthCheck();
}

OPENGEMINI_INLINE_SPECIFIER
ClientImpl::~ClientImpl()
{
    lb_->StopHealthCheck();
    ctx_.Shutdown();
}

OPENGEMINI_INLINE_SPECIFIER
std::shared_ptr<http::IHttpClient>
ClientImpl::ConstructHttpClient(const ClientConfig& config)
{
    std::shared_ptr<http::IHttpClient> http;
#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
    if (config.tlsEnabled) {
        http = std::make_shared<http::HttpsClient>(
            ctx_(),
            config.connectTimeout,
            config.timeout,
            config.tlsConfig.value_or(TLSConfig{}));
    }
    else
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT
    {
        http = std::make_shared<http::HttpClient>(ctx_(),
                                                  config.connectTimeout,
                                                  config.timeout);
    }

    if (auto& auth = config.authConfig; auth.has_value()) {
        auto val = std::get_if<AuthCredential>(&auth.value());
        if (!val) {
            throw Exception(errc::LogicErrors::NotImplemented,
                            "Only support authorization credential");
        }

        auto cred = fmt::format("{}:{}", val->username, val->password);
        http->DefaultHeaders()["Authorization"] =
            fmt::format("Basic {}", util::Base64Encode(cred));
    }

    return http;
};

} // namespace opengemini::impl

#include "opengemini/impl/cli/Database.cpp"
#include "opengemini/impl/cli/Ping.cpp"
#include "opengemini/impl/cli/Query.cpp"
#include "opengemini/impl/cli/RetentionPolicy.cpp"
