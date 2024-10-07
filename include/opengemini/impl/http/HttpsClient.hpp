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

#ifndef OPENGEMINI_IMPL_HTTP_HTTPSCLIENT_HPP
#define OPENGEMINI_IMPL_HTTP_HTTPSCLIENT_HPP

#include <boost/beast/ssl.hpp>

#include "opengemini/ClientConfig.hpp"
#include "opengemini/impl/http/ConnectionPool.hpp"
#include "opengemini/impl/http/IHttpClient.hpp"

namespace opengemini::impl::http {

class HttpsClient : public IHttpClient {
public:
    explicit HttpsClient(boost::asio::io_context&  ctx,
                         std::chrono::milliseconds connectTimeout,
                         std::chrono::milliseconds readWriteTimeout,
                         const TLSConfig&          tlsConfig);
    ~HttpsClient() = default;

private:
    class Pool :
        public ConnectionPool<
            Pool,
            boost::beast::ssl_stream<boost::beast::tcp_stream>> {
        friend class ConnectionPool<Pool, Stream>;

    public:
        Pool(boost::asio::io_context&   ctx,
             std::chrono::milliseconds  connectTimeout,
             boost::asio::ssl::context& sslCtx);

    private:
        ConnectionPtr CreateConnection(const Endpoint&            endpoint,
                                       boost::asio::yield_context yield);

    private:
        boost::asio::ssl::context& sslCtx_;
    };

private:
    Response SendRequest(const Endpoint&            endpoint,
                         Request                    request,
                         boost::asio::yield_context yield) override;

private:
    boost::asio::ssl::context sslCtx_;
    Pool                      pool_;
};

} // namespace opengemini::impl::http

#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/http/HttpsClient.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION

#endif // !OPENGEMINI_IMPL_HTTP_HTTPSCLIENT_HPP
