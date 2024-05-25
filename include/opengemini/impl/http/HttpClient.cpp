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

#include "opengemini/impl/http/HttpClient.hpp"

#include "opengemini/Exception.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl::http {

OPENGEMINI_INLINE_SPECIFIER
HttpClient::HttpClient(boost::asio::io_context&  ctx,
                       std::chrono::milliseconds connectTimeout,
                       std::chrono::milliseconds readWriteTimeout) :

    IHttpClient(ctx, connectTimeout, readWriteTimeout),
    pool_(ctx, connectTimeout)
{ }

OPENGEMINI_INLINE_SPECIFIER
Response HttpClient::SendRequest(const Endpoint&            endpoint,
                                 Request                    request,
                                 boost::asio::yield_context yield)
{
    namespace beast = boost::beast;
    namespace http  = boost::beast::http;

    Response           response;
    beast::flat_buffer buffer;
    beast::error_code  error;

    for (;; error.clear()) {
        auto  connection = pool_.Retrieve(endpoint, yield);
        auto& stream     = connection->stream;

        stream.expires_after(readWriteTimeout_);
        http::async_write(stream, request, yield[error]);
        if (connection->ShouldRetry(error, "Write to stream failed.")) {
            continue;
        }

        buffer.clear();
        http::async_read(stream, buffer, response, yield[error]);
        if (connection->ShouldRetry(error, "Read from stream failed.")) {
            continue;
        }

        if (response.keep_alive()) {
            pool_.Push(endpoint, std::move(connection));
            break;
        }

        std::ignore = stream.socket().shutdown(
            boost::asio::ip::tcp::socket::shutdown_both,
            error);
        if (error && error != beast::errc::not_connected) {
            throw Exception(error, "Shutdown stream failed.");
        }
        break;
    }

    return response;
}

OPENGEMINI_INLINE_SPECIFIER
HttpClient::Pool::Pool(boost::asio::io_context&  ctx,
                       std::chrono::milliseconds connectTimeout) :
    ConnectionPool(ctx, connectTimeout)
{ }

OPENGEMINI_INLINE_SPECIFIER
HttpClient::Pool::ConnectionPtr
HttpClient::Pool::CreateConnection(const Endpoint&            endpoint,
                                   boost::asio::yield_context yield)
{
    boost::asio::ip::tcp::resolver resolver(ctx_);
    boost::beast::error_code       error;
    auto results = resolver.async_resolve(endpoint.host,
                                          std::to_string(endpoint.port),
                                          yield[error]);
    if (error) { throw Exception(error, "Resolve hostname failed."); }

    auto connection =
        std::make_unique<ConnectionPtr::element_type>(Stream{ ctx_ }, false);
    auto& stream = connection->stream;
    stream.expires_after(connectTimeout_);
    stream.async_connect(results, yield[error]);
    if (error) { throw Exception(error, "Connect to server failed."); }

    return connection;
}

} // namespace opengemini::impl::http
