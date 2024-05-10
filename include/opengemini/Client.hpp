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

namespace opengemini {

namespace impl {
class ClientImpl;
}

///
/// \~English
/// @brief An openGemini client.
///
/// \~Simplified Chinese
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
    /// \~Simplified Chinese
    /// @brief 构造函数。
    /// @details 创建一个能与openGemini数据库集群通讯的客户端。
    /// @param config 客户端配置，可以直接构造该结构体 @ref ClientConfig
    /// ，或通过 @ref ClientConfigBuilder 构建。
    ///
    explicit Client(const ClientConfig& config);
    ~Client() = default;

    Client(Client&& client) noexcept;
    Client& operator=(Client&& client) noexcept;

private:
    Client(const Client&)            = delete;
    Client& operator=(const Client&) = delete;

private:
    std::unique_ptr<impl::ClientImpl> impl_;
};

} // namespace opengemini

#include "opengemini/impl/Client.ipp"

#endif // !OPENGEMINI_CLIENT_HPP
