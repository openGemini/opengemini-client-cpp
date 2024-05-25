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

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT

// clang-format off
#include "opengemini/impl/http/HttpsClient.hpp"

#include "opengemini/Exception.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"
// clang-format on

namespace opengemini::impl::http {

OPENGEMINI_INLINE_SPECIFIER
HttpsClient::HttpsClient(boost::asio::io_context&  ctx,
                         std::chrono::milliseconds connectTimeout,
                         std::chrono::milliseconds readWriteTimeout,
                         const TLSConfig&          tlsConfig) :
    IHttpClient(ctx, connectTimeout, readWriteTimeout),
    sslCtx_(static_cast<boost::asio::ssl::context::method>(tlsConfig.version)),
    pool_(ctx, connectTimeout_, sslCtx_)
{
    try {
        if (tlsConfig.rootCAs.empty()) { sslCtx_.set_default_verify_paths(); }
        else {
            sslCtx_.add_certificate_authority(
                boost::asio::buffer(tlsConfig.rootCAs.data(),
                                    tlsConfig.rootCAs.size()));
        }

        if (!tlsConfig.certificates.empty()) {
            sslCtx_.use_certificate_chain(
                boost::asio::buffer(tlsConfig.certificates.data(),
                                    tlsConfig.certificates.size()));
        }

        sslCtx_.set_verify_mode(tlsConfig.skipVerifyPeer
                                    ? boost::asio::ssl::verify_none
                                    : boost::asio::ssl::verify_peer);
    }
    catch (const boost::system::system_error& e) {
        throw Exception(e.code(), "Initialize TLS configuration failed.");
    }
}

OPENGEMINI_INLINE_SPECIFIER
Response HttpsClient::SendRequest(const Endpoint&            endpoint,
                                  Request                    request,
                                  boost::asio::yield_context yield)
{
    namespace asio  = boost::asio;
    namespace beast = boost::beast;
    namespace http  = boost::beast::http;

    Response           response;
    beast::flat_buffer buffer;
    beast::error_code  error;

    for (;; error.clear()) {
        auto  connection = pool_.Retrieve(endpoint, yield);
        auto& tlsStream  = connection->stream;
        auto& tcpStream  = beast::get_lowest_layer(tlsStream);

        tcpStream.expires_after(readWriteTimeout_);
        http::async_write(tlsStream, request, yield[error]);
        if (connection->ShouldRetry(error, "Write to stream failed.")) {
            continue;
        }

        buffer.clear();
        http::async_read(tlsStream, buffer, response, yield[error]);
        if (connection->ShouldRetry(error, "Read from stream failed.")) {
            continue;
        }

        if (response.keep_alive()) {
            pool_.Push(endpoint, std::move(connection));
            break;
        }

        tlsStream.async_shutdown(yield[error]);
        if (error && error != asio::error::eof &&
            error != asio::ssl::error::stream_truncated) {
            throw Exception(error, "Shutdown stream failed.");
        }
        break;
    }

    return response;
}

OPENGEMINI_INLINE_SPECIFIER
HttpsClient::Pool::Pool(boost::asio::io_context&   ctx,
                        std::chrono::milliseconds  connectTimeout,
                        boost::asio::ssl::context& sslCtx) :
    ConnectionPool(ctx, connectTimeout),
    sslCtx_(sslCtx)
{ }

OPENGEMINI_INLINE_SPECIFIER
HttpsClient::Pool::ConnectionPtr
HttpsClient::Pool::CreateConnection(const Endpoint&            endpoint,
                                    boost::asio::yield_context yield)
{
    namespace asio  = boost::asio;
    namespace beast = boost::beast;

    auto& [host, port] = endpoint;
    boost::beast::error_code error;
    asio::ip::tcp::resolver  resolver(ctx_);

    auto results =
        resolver.async_resolve(host, std::to_string(port), yield[error]);
    if (error) { throw Exception(error, "Resolve hostname failed."); }

    auto connection =
        std::make_unique<ConnectionPtr::element_type>(Stream{ ctx_, sslCtx_ },
                                                      false);

    auto& sslStream = connection->stream;
    if (!SSL_set_tlsext_host_name(sslStream.native_handle(), host.c_str())) {
        error.assign(static_cast<int>(::ERR_get_error()),
                     asio::error::get_ssl_category());
        throw Exception(error, "Set TLS server name failed.");
    }

    auto& tcpStream = beast::get_lowest_layer(sslStream);
    tcpStream.expires_after(connectTimeout_);
    tcpStream.async_connect(results, yield[error]);
    if (error) { throw Exception(error, "Connect to server failed."); }

    sslStream.async_handshake(asio::ssl::stream_base::client, yield[error]);
    if (error) { throw Exception(error, "Perform TLS handshake failed."); }

    return connection;
}

} // namespace opengemini::impl::http

#endif // OPENGEMINI_ENABLE_SSL_SUPPORT
