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

/// @file ClientConfig.hpp
#ifndef OPENGEMINI_CLIENTCONFIG_HPP
#define OPENGEMINI_CLIENTCONFIG_HPP

#include <chrono>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
#    include <boost/asio/ssl.hpp>
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT

#include "opengemini/Endpoint.hpp"

namespace opengemini {

///
/// \~English
/// @brief Holds the user credential for authorization.
///
/// \~Chinese
/// @brief 存放鉴权使用的用户凭据。
///
struct AuthCredential {
    std::string username;
    std::string password;
};

///
/// \~English
/// @brief Authentication token.
///
/// \~Chinese
/// @brief 鉴权令牌。
///
using AuthToken = std::string;

///
/// \~English
/// @brief Authentication configuration, which can only be either user
/// credential or token.
///
/// \~Chinese
/// @brief 鉴权配置，只能是用户凭据或者用户令牌中的一种。
///
using AuthConfig = std::variant<AuthCredential, AuthToken>;

///
/// \~English
/// @brief Hold the configs that control the batching policy.
/// @detail When calling @ref WritePoint() , the client may not send the
/// request immediately (but waiting for more calls to gather into one
/// request). This structure is used to control the specific policy for
/// batching.
///
/// \~Chinese
/// @brief 批量配置，控制写入点位时的批量策略。
/// @detail 调用 @ref WritePoint() 时，
/// 客户端可能不会立刻发送请求（而是等待更多调用以聚合为一次请求）。
/// 该结构体用于控制批量请求的具体策略。
///
struct BatchConfig {
    ///
    /// \~English
    /// @brief Time interval that triggers a batching request.
    /// @details An aggregated request will be sent immediately if the timer
    /// has expired.
    ///
    /// \~Chinese
    /// @brief 触发批量请求的时间间隔。
    /// @details 若定时器到期，则立即发送一次聚合请求。
    ///
    std::chrono::milliseconds batchInterval;

    ///
    /// \~English
    /// @brief Max number of points that triggers a gather request.
    /// @details A aggregated request will be sent immediately if the number of
    /// points exceeds the maximum size.
    ///
    /// \~Chinese
    /// @brief 触发批量请求的最大点位数量。
    /// @details 如果累计的点位数量超出最大值，则立即发送一次聚合请求。
    ///
    std::size_t batchSize;
};

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT

///
/// \~English
/// @brief The version of SSL/TLS protocol.
///
/// \~Chinese
/// @brief SSL/TLS协议版本。
///
enum class TLSVersion {
    sslv2  = boost::asio::ssl::context::sslv2_client,
    sslv3  = boost::asio::ssl::context::sslv3_client,
    tlsv1  = boost::asio::ssl::context::tlsv1_client,
    tlsv11 = boost::asio::ssl::context::tlsv11_client,
    tlsv12 = boost::asio::ssl::context::tlsv12_client,
    tlsv13 = boost::asio::ssl::context::tlsv13_client,
};

///
/// \~English
/// @brief The TLS configuration for client.
///
/// \~Chinese
/// @brief 客户端TLS配置。
///
struct TLSConfig {
    ///
    /// \~English
    /// @brief Whether to skip verifying the server's certificates, default to
    /// false.
    ///
    /// \~Chinese
    /// @brief 是否跳过验证服务端证书，默认值为false。
    ///
    bool skipVerifyPeer{ false };

    ///
    /// \~English
    /// @brief A PEM format certificate chain as the client's certificates,
    /// default to an empty string(no client's certificates used).
    ///
    /// \~Chinese
    /// @brief PEM格式的客户端证书链，默认值为空（不使用客户端证书）。
    ///
    std::string certificates;

    ///
    /// \~English
    /// @brief The certificate authority for performing verification.
    /// @details Must be a certificate chain in PEM format. If not specified,
    /// the client will use CAs under the default system-dependent directories.
    ///
    /// \~Chinese
    /// @brief 校验服务端证书时使用的根CA。
    /// @details
    /// 必须是PEM格式的证书链。若没有指定该选项，则客户端将使用系统默认路径上存在的根CA。
    ///
    std::string rootCAs;

    ///
    /// \~English
    /// @brief SSL/TLS version to use on the client, default to TLS v1.2.
    ///
    /// \~Chinese
    /// @brief 客户端使用的SSL/TLS版本，默认使用TLS v1.2。
    ///
    TLSVersion version{ TLSVersion::tlsv12 };
};

#endif // OPENGEMINI_ENABLE_SSL_SUPPORT

///
/// \~English
/// @brief The configuration of openGemini client.
///
/// \~Chinese
/// @brief openGemini客户端配置。
///
struct ClientConfig {
    ///
    /// \~English
    /// @brief Addresses of openGemini server.
    /// @details Consist of one or more endpoints @ref Endpoint , and is not
    /// allowed to be empty or to contain duplicate endpoints.
    ///
    /// \~Chinese
    /// @brief openGemini服务端地址列表。
    /// @details 可由一个或多个端点 @ref Endpoint
    /// 组成，不允许为空，也不允许包含重复端点。
    ///
    std::vector<Endpoint> addresses;

    ///
    /// \~English
    /// @brief Client authentication, default to @code std::nullopt @endcode (no
    /// authorization required).
    ///
    /// \~Chinese
    /// @brief 客户端鉴权配置，默认值为 @code std::nullopt
    /// @endcode（无需鉴权）。
    ///
    std::optional<AuthConfig> authConfig{ std::nullopt };

    ///
    /// \~English
    /// @brief Batching configuration, default to @code std::nullopt
    /// @endcode (All of @ref WritePoint requests will be sent immediately
    /// instead of being aggregated).
    ///
    /// \~Chinese
    /// @brief 批量配置，默认值为 @code std::nullopt @endcode
    /// （所有的@ref WritePoint 请求立即被发送，不会被聚合）。
    ///
    std::optional<BatchConfig> batchConfig{ std::nullopt };

    ///
    /// \~English
    /// @brief Client read/write timeout, default to 30 seconds.
    ///
    /// \~Chinese
    /// @brief 客户端读写超时，默认值为30秒。
    ///
    std::chrono::milliseconds timeout{ std::chrono::seconds(30) };

    ///
    /// \~English
    /// @brief Client connect timeout, default to 30 seconds.
    ///
    /// \~Chinese
    /// @brief 客户端连接超时，默认值为30秒。
    ///
    std::chrono::milliseconds connectTimeout{ std::chrono::seconds(30) };

    ///
    /// \~English
    /// @brief Whether to enable gzip, default to false.
    ///
    /// \~Chinese
    /// @brief 是否开启gzip，默认值为false。
    ///
    bool gzipEnabled{ false };

    ///
    /// \~English
    /// @brief A hint about the level of concurrency.
    /// @details A suggestion to the client on how many threads it should allow
    /// to run, default to 0 (at the client's discretion).
    ///
    /// \~Chinese
    /// @brief 客户端并发参考值。
    /// @details 期望客户端运行的线程数，
    /// 客户端可能参考该值选择合适的线程数。默认值为0（由客户端自行决定）。
    ///
    std::size_t concurrencyHint{ 0 };
#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT

    ///
    /// \~English
    /// @brief The Client TLS configuration, default to @code std::nullopt
    /// @endcode (Client will use the default value of @ref TLSConfig ).
    ///
    /// \~Chinese
    /// @brief 客户端TLS配置，默认值为@code std::nullopt @endcode
    /// (客户端内部将使用 @ref TLSConfig 默认值)。
    ///
    std::optional<TLSConfig> tlsConfig{ std::nullopt };

    ///
    /// \~English
    /// @brief Whether to enable TLS, default to false.
    ///
    /// \~Chinese
    /// @brief 是否开启TLS，默认值为false。
    ///
    bool tlsEnabled{ false };

#endif // OPENGEMINI_ENABLE_SSL_SUPPORT
};

} // namespace opengemini

#endif // !OPENGEMINI_CLIENTCONFIG_HPP
