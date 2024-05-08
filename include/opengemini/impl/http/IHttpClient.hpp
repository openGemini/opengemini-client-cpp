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

#ifndef OPENGEMINI_IMPL_HTTP_IHTTPCLIENT_HPP
#define OPENGEMINI_IMPL_HTTP_IHTTPCLIENT_HPP

#include <chrono>
#include <unordered_map>

#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>

#include "opengemini/Endpoint.hpp"
#include "opengemini/impl/comm/TaskSlot.hpp"

namespace opengemini::impl::http {

using Status   = boost::beast::http::status;
using Request  = boost::beast::http::request<boost::beast::http::string_body>;
using Response = boost::beast::http::response<boost::beast::http::string_body>;

class IHttpClient : public TaskSlot {
public:
    IHttpClient(boost::asio::io_context&  ctx,
                std::chrono::milliseconds connectTimeout,
                std::chrono::milliseconds readWriteTimeout);

    virtual ~IHttpClient() = default;

    Response Get(const Endpoint&            endpoint,
                 std::string                target,
                 boost::asio::yield_context yield);

    Response Post(const Endpoint&            endpoint,
                  std::string                target,
                  std::string                body,
                  boost::asio::yield_context yield);

    std::unordered_map<std::string, std::string>& DefaultHeaders() noexcept;

protected:
    virtual Response SendRequest(const Endpoint&            endpoint,
                                 Request                    request,
                                 boost::asio::yield_context yield) = 0;

private:
    Request BuildRequest(std::string              host,
                         std::string              target,
                         std::string              body,
                         boost::beast::http::verb method) const;

protected:
    const std::chrono::milliseconds connectTimeout_;
    const std::chrono::milliseconds readWriteTimeout_;

private:
    std::unordered_map<std::string, std::string> headers_;

    const std::string     userAgent_;
    static constexpr auto httpProtocolVersion_{ 11 };
};

} // namespace opengemini::impl::http

#ifndef OPENGEMINI_SEPERATE_COMPILATION
#    include "opengemini/impl/http/IHttpClient.cpp"
#endif // !OPENGEMINI_SEPERATE_COMPILATION

#endif // !OPENGEMINI_IMPL_HTTP_IHTTPCLIENT_HPP
