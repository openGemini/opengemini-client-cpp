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

#ifndef OPENGEMINI_IMPL_HTTP_HTTPCLIENT_HPP
#define OPENGEMINI_IMPL_HTTP_HTTPCLIENT_HPP

#include "opengemini/impl/http/ConnectionPool.hpp"
#include "opengemini/impl/http/IHttpClient.hpp"

namespace opengemini::impl::http {

class HttpClient : public IHttpClient {
public:
    explicit HttpClient(boost::asio::io_context&  ctx,
                        std::chrono::milliseconds connectTimeout,
                        std::chrono::milliseconds readWriteTimeout);
    ~HttpClient() = default;

private:
    class Pool : public ConnectionPool<Pool, boost::beast::tcp_stream> {
        friend class ConnectionPool<Pool, Stream>;

    public:
        Pool(boost::asio::io_context&  ctx,
             std::chrono::milliseconds connectTimeout);

    private:
        ConnectionPtr CreateConnection(const Endpoint&            endpoint,
                                       boost::asio::yield_context yield);
    };

private:
    Response SendRequest(const Endpoint&            endpoint,
                         Request                    request,
                         boost::asio::yield_context yield) override;

private:
    Pool pool_;
};

} // namespace opengemini::impl::http

#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/http/HttpClient.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION

#endif // !OPENGEMINI_IMPL_HTTP_HTTPCLIENT_HPP
