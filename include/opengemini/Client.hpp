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

#ifndef OPENGEMINI_CLIENT_HPP
#define OPENGEMINI_CLIENT_HPP

#include <memory>

#include "opengemini/ClientConfig.hpp"
#include "opengemini/CompletionToken.hpp"
#include "opengemini/Query.hpp"

namespace opengemini {

namespace impl {
class ClientImpl;
}

///
/// \~English
/// @brief An openGemini client.
///
/// \~Chinese
/// @brief openGemini客户端。
///
class Client {
public:
    ///
    /// \~English
    /// @brief A constructor.
    /// @details Creates a client that can be used to communicate with the
    /// openGemini cluster.
    /// @param config The client's configuration, which can be directly
    /// constructed as @ref ClientConfig structure, or can be built using @ref
    /// ClientConfigBuilder.
    ///
    /// \~Chinese
    /// @brief 构造函数。
    /// @details 创建一个能与openGemini数据库集群通讯的客户端。
    /// @param config 客户端配置，可以直接构造该结构体 @ref ClientConfig
    /// ，或通过 @ref ClientConfigBuilder 构建。
    ///
    explicit Client(const ClientConfig& config);
    ~Client() = default;

    Client(Client&& client) noexcept;
    Client& operator=(Client&& client) noexcept;

    ///
    /// \~English
    /// @brief Check the status of the database.
    /// @details Check connectivity with specified database through sending ping
    /// request, and return the version of the server.
    /// @param index Index of database.
    /// @param token The completion token which will be invoked when the task
    /// complete. Default to @ref token::sync if this parameter is not
    /// specified. If passing function object as token, the function signature
    /// must be:
    /// @code
    /// void (
    ///     // Result of operation, it means success if the value is nullptr,
    ///     // otherwise, contains an exception.
    ///     std::exception_ptr error,
    ///     // On success, the version of OpenGemini server.
    ///     std::string version
    /// )
    /// @endcode
    ///
    /// \~Chinese
    /// @brief 检查数据库的连接状态。
    /// @details
    /// 发送Ping请求以测试指定数据库的连接是否正常，将返回服务端的版本号。
    /// @param index 数据库集群索引。
    /// @param token 任务完成令牌，将在任务完成后被调用。
    /// 若没有指定该参数，则使用默认值：@ref token::sync 。
    /// 若传递函数对象作为完成令牌，则其签名必须满足：
    /// @code
    /// void (
    ///     // 执行结果，仅当值为nullptr时代表成功，否则将承载相关的异常。
    ///     std::exception_ptr error,
    ///     // 当操作成功时，承载服务端的版本号。
    ///     std::string version
    /// )
    /// @endcode
    ///
    template<typename COMPLETION_TOKEN = token::Sync>
    [[nodiscard]] auto Ping(std::size_t index, COMPLETION_TOKEN&& token = {});

    ///
    /// \~English
    /// @brief Query data from database.
    /// @details Query data stored in database,
    /// and return the results as @ref QueryResult.
    /// @param query The query statement as @ref struct Query.
    /// @param token The completion token which will be invoked when the task
    /// complete. Default to @ref token::sync if this parameter is not
    /// specified. If passing function object as token, the function signature
    /// must be:
    /// @code
    /// void (
    ///     // Result of operation, it means success if the value is nullptr,
    ///     // otherwise, contains an exception.
    ///     std::exception_ptr error,
    ///     // On success, the query result.
    ///     QueryResult result
    /// )
    /// @endcode
    ///
    /// \~Chinese
    /// @brief 从数据库查询数据。
    /// @details
    /// 查询数据库中存储的数据，将返回查询结果 @ref QueryResult 。
    /// @param query 查询语句 @ref struct Query 。
    /// @param token 任务完成令牌，将在任务完成后被调用。
    /// 若没有指定该参数，则使用默认值：@ref token::sync 。
    /// 若传递函数对象作为完成令牌，则其签名必须满足：
    /// @code
    /// void (
    ///     // 执行结果，仅当值为nullptr时代表成功，否则将承载相关的异常。
    ///     std::exception_ptr error,
    ///     // 当操作成功时，承载查询结果。
    ///     QueryResult result
    /// )
    /// @endcode
    ///
    template<typename COMPLETION_TOKEN = token::Sync>
    [[nodiscard]] auto Query(struct Query query, COMPLETION_TOKEN&& token = {});

private:
    Client(const Client&)            = delete;
    Client& operator=(const Client&) = delete;

private:
    std::unique_ptr<impl::ClientImpl> impl_;
};

} // namespace opengemini

#include "opengemini/impl/Client.ipp"

#endif // !OPENGEMINI_CLIENT_HPP
