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

#ifndef OPENGEMINI_CLIENTCONFIGBUILDER_HPP
#define OPENGEMINI_CLIENTCONFIGBUILDER_HPP

#include "opengemini/ClientConfig.hpp"

namespace opengemini {

///
/// \~English
/// @brief Class to build client configuration.
/// @details For details about fileds of configuration see @ref ClientConfig.
/// @note The member function @ref Finalize() must be called at the end to get
/// the configuration object.
///
/// \~Simplified Chinese
/// @brief 用于构建客户端配置的类。
/// @details 客户端配置各字段的具体含义 @ref ClientConfig 。
/// @note 必须在最后调用成员函数
/// @ref Finalize() 以获取最终的配置对象。
///
class ClientConfigBuilder {
private:
    using Self = ClientConfigBuilder;

public:
    ///
    /// \~English
    /// @brief Default constructor.
    ///
    /// \~Simplified Chinese
    /// @brief 默认构造函数。
    ///
    ClientConfigBuilder()  = default;
    ~ClientConfigBuilder() = default;

    ///
    /// \~English
    /// @brief Finish bulding the client configuration and return it.
    /// @return The config struct as built.
    ///
    /// \~Simplified Chinese
    /// @brief 结束构造并返回相应的配置。
    /// @return 客户端配置结构体。
    ///
    const ClientConfig& Finalize() const noexcept;

    ///
    /// \~English
    /// @brief Append a server address to which the client will connect.
    /// @param address An @ref Endpoint to server.
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 增加一个服务器地址以供客户端连接。
    /// @param address 服务器端点 @ref Endpoint。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& AppendAddress(const Endpoint& address);

    ///
    /// \~English
    /// @brief Append multiple server addresses to which the client will
    /// connect.
    /// @param addresses A vector of @ref Endpoint.
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 增加多个服务器地址以供客户端连接。
    /// @param addresses 服务器端点 @ref Endpoint 列表。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& AppendAddresses(const std::vector<Endpoint>& addresses);

    ///
    /// \~English
    /// @brief Set the credential used for authentication.
    /// @see AuthCredential
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置鉴权时使用的用户凭据。
    /// @see AuthCredential
    /// @return 指向配置构造器自身的引用。
    ///
    Self& AuthCredential(const std::string& username,
                         const std::string& password);

    ///
    /// \~English
    /// @brief Set the token used for authentication.
    /// @see AuthToken
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置鉴权时使用的用户令牌。
    /// @see AuthToken
    /// @return 指向配置构造器自身的引用。
    ///
    Self& AuthToken(const std::string& token);

    ///
    /// \~English
    /// @brief Set the strategy for write-point batching.
    /// @see BatchConfig
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置点位写入功能的批量策略。
    /// @see BatchConfig
    /// @return 指向配置构造器自身的引用。
    ///
    Self& BatchConfig(std::chrono::milliseconds interval, std::size_t size);

    ///
    /// \~English
    /// @brief Set the read/write timeout.
    /// @param timeout
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置客户端读写超时。
    /// @param timeout 超时值。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& ReadWriteTimeout(std::chrono::milliseconds timeout);

    ///
    /// \~English
    /// @brief Set the connect timeout.
    /// @param timeout
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置客户端连接超时。
    /// @param 超时值。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& ConnectTimeout(std::chrono::milliseconds timeout);

    ///
    /// \~English
    /// @brief Set whether to enable gzip or not.
    /// @param enabled
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置是否开启gzip。
    /// @param enabled
    /// @return 指向配置构造器自身的引用。
    ///
    Self& EnableGzip(bool enabled);

    ///
    /// \~English
    /// @brief Set the hint about the level of concurrency.
    /// @param hint A suggestion to the client on how many threads it should
    /// allow to run.
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置客户端并发参考值。
    /// @param hint 期望客户端运行的线程数。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& ConcurrencyHint(std::size_t hint);

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
    ///
    /// \~English
    /// @brief Set whehter to skip verifying the server's certificates or not.
    /// @param skippped
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置是否跳过校验服务端证书。
    /// @param skipped
    /// @return 指向配置构造器自身的引用。
    ///
    Self& SkipVerifyPeer(bool skipped);

    ///
    /// \~English
    /// @brief Set a certificate chain as the client's certificates.
    /// @param certificates A PEM format certificate chain.
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置客户端证书。
    /// @param certificates PEM格式的证书链。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& ClientCertificates(const std::string& certificates);

    ///
    /// \~English
    /// @brief Set the trusted certificate authority for performing verfication.
    /// @param rootCAs A certificate chain in PEM format.
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置校验服务端证书时使用的根CA。
    /// @param rootCAs PEM格式的证书链。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& RootCAs(const std::string& rootCAs);

    ///
    /// \~English
    /// @brief Set version of SSL/TLS protocol.
    /// @param version
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置SSL/TLS版本。
    /// @param version 版本。
    /// @return 指向配置构造器自身的引用。
    ///
    Self& TLSVersion(TLSVersion version);

    ///
    /// \~English
    /// @brief Set whether to enable SSL/TLS or not.
    /// @param enabled
    /// @return Reference to the builder itself.
    ///
    /// \~Simplified Chinese
    /// @brief 设置是否开启SSL/TLS。
    /// @param enabled
    /// @return 指向配置构造器自身的引用。
    ///
    Self& EnableTLS(bool enabled);
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT

private:
    ClientConfigBuilder(const ClientConfigBuilder&)                = delete;
    ClientConfigBuilder(ClientConfigBuilder&&) noexcept            = delete;
    ClientConfigBuilder& operator=(const ClientConfigBuilder&)     = delete;
    ClientConfigBuilder& operator=(ClientConfigBuilder&&) noexcept = delete;

#ifdef OPENGEMINI_ENABLE_SSL_SUPPORT
    TLSConfig& PrepareTLSConfig();
#endif // OPENGEMINI_ENABLE_SSL_SUPPORT

    ClientConfig conf_;
};

} // namespace opengemini

#ifndef OPENGEMINI_SEPERATE_COMPILATION
#    include "opengemini/impl/ClientConfigBuilder.cpp"
#endif // !OPENGEMINI_SEPERATE_COMPILATION

#endif // !OPENGEMINI_CLIENTCONFIGBUILDER_HPP
