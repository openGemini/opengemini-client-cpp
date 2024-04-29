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

#ifndef OPENGEMINI_ENDPOINT_HPP
#define OPENGEMINI_ENDPOINT_HPP

#include <string>

#include <boost/functional/hash.hpp>

namespace opengemini {

///
/// \~English
/// @brief Address of openGemini server.
///
/// \~Chinese
/// @brief openGemini服务器地址。
///
struct Endpoint {
    std::string host;
    uint16_t    port;

#if __cplusplus < 202002L
    Endpoint() = default;

    Endpoint(std::string _host, uint16_t _port) :
        host(std::move(_host)),
        port(_port)
    { }
#endif // (__cplusplus < 202002L)

    friend bool operator==(const Endpoint& lhs, const Endpoint& rhs) noexcept
    {
        return (lhs.port == rhs.port) && (lhs.host == rhs.host);
    }

    friend std::ostream& operator<<(std::ostream& os, const Endpoint& endpoint)
    {
        os << endpoint.host << ":" << endpoint.port;
        return os;
    }

    struct Hasher {
        std::size_t operator()(const Endpoint& endpoint) const
        {
            std::size_t hash{ 0 };
            boost::hash_combine(hash, boost::hash_value(endpoint.host));
            boost::hash_combine(hash, boost::hash_value(endpoint.port));
            return hash;
        }
    };
};

} // namespace opengemini

#endif // !OPENGEMINI_ENDPOINT_HPP
