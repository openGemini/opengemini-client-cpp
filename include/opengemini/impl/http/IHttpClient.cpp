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

#include "opengemini/impl/http/IHttpClient.hpp"

#include <fmt/format.h>

#include "opengemini/Version.hpp"
#include "opengemini/impl/util/ErrorHandling.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl::http {

OPENGEMINI_INLINE_SPECIFIER
IHttpClient::IHttpClient(boost::asio::io_context&  ctx,
                         std::chrono::milliseconds connectTimeout,
                         std::chrono::milliseconds readWriteTimeout) :
    TaskSlot(ctx),
    connectTimeout_(connectTimeout),
    readWriteTimeout_(readWriteTimeout),
    userAgent_([] {
        return fmt::format("opengemini-client-cxx/{}.{}.{}",
                           OPENGEMINI_VERSION_MAJOR,
                           OPENGEMINI_VERSION_MINOR,
                           OPENGEMINI_VERSION_PATCH);
    }())
{ }

OPENGEMINI_INLINE_SPECIFIER
Response IHttpClient::Get(Endpoint                   endpoint,
                          std::string                target,
                          boost::asio::yield_context yield)
{
    auto request = BuildRequest(endpoint.host,
                                std::move(target),
                                {},
                                boost::beast::http::verb::get);
    return SendRequest(std::move(endpoint), std::move(request), yield);
}

OPENGEMINI_INLINE_SPECIFIER
Response IHttpClient::Get(Endpoint                   endpoint,
                          std::string                target,
                          boost::asio::yield_context yield,
                          Error&                     error)
try {
    error.Clear();
    return Get(std::move(endpoint), std::move(target), yield);
}
catch (...) {
    util::ConvertError(error);
    return {};
}

OPENGEMINI_INLINE_SPECIFIER
Response IHttpClient::Post(Endpoint                   endpoint,
                           std::string                target,
                           std::string                body,
                           boost::asio::yield_context yield)
{
    auto request = BuildRequest(endpoint.host,
                                std::move(target),
                                std::move(body),
                                boost::beast::http::verb::post);
    return SendRequest(std::move(endpoint), std::move(request), yield);
}

OPENGEMINI_INLINE_SPECIFIER
Response IHttpClient::Post(Endpoint                   endpoint,
                           std::string                target,
                           std::string                body,
                           boost::asio::yield_context yield,
                           Error&                     error)
try {
    error.Clear();
    return Post(std::move(endpoint), std::move(target), std::move(body), yield);
}
catch (...) {
    util::ConvertError(error);
    return {};
}

OPENGEMINI_INLINE_SPECIFIER
std::unordered_map<std::string, std::string>&
IHttpClient::DefaultHeaders() noexcept
{
    return headers_;
}

OPENGEMINI_INLINE_SPECIFIER
Request IHttpClient::BuildRequest(std::string              host,
                                  std::string              target,
                                  std::string              body,
                                  boost::beast::http::verb method) const
{
    Request request{ std::move(method),
                     std::move(target),
                     httpProtocolVersion_ };
    request.body() = std::move(body);
    for (const auto& header : headers_) {
        request.set(header.first, header.second);
    }
    request.set(boost::beast::http::field::host, std::move(host));
    request.set(boost::beast::http::field::user_agent, userAgent_);
    request.prepare_payload();

    return request;
}

} // namespace opengemini::impl::http
