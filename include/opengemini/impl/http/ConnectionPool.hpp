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

#ifndef OPENGEMINI_IMPL_HTTP_CONNECTIONPOOL_HPP
#define OPENGEMINI_IMPL_HTTP_CONNECTIONPOOL_HPP

#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>

#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>

#include "opengemini/Endpoint.hpp"
#include "opengemini/impl/comm/TaskSlot.hpp"

namespace opengemini::impl::http {

template<typename STREAM>
struct Connection {
    using Stream = STREAM;

    Stream stream;
    bool   used;

    bool ShouldRetry(boost::beast::error_code& error,
                     std::string_view          what) const;

#if __cplusplus < 202002L
    Connection(Stream _stream, bool _used);
#endif // (__cplusplus < 202002L)
};

template<typename DERIVED, typename STREAM>
class ConnectionPool : public TaskSlot {
public:
    using ConnectionPtr = std::unique_ptr<Connection<STREAM>>;
    using Stream        = STREAM;

public:
    ConnectionPool(boost::asio::io_context&  ctx,
                   std::chrono::milliseconds connectTimeout,
                   std::size_t               maxKeepalive = 3);

    ConnectionPtr Retrieve(const Endpoint&            endpoint,
                           boost::asio::yield_context yield);

    void Push(const Endpoint& endpoint, ConnectionPtr connection);

protected:
    const std::chrono::milliseconds connectTimeout_;

private:
    std::unordered_map<Endpoint, std::queue<ConnectionPtr>, Endpoint::Hasher>
               pool_;
    std::mutex mutex_;

    const std::size_t maxKeepalive_;
};

} // namespace opengemini::impl::http

#include "opengemini/impl/http/ConnectionPool.tpp"

#endif // !OPENGEMINI_IMPL_HTTP_CONNECTIONPOOL_HPP
