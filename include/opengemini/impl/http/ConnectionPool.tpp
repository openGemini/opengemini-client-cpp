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

#include "opengemini/impl/http/ConnectionPool.hpp"

#include "opengemini/Exception.hpp"

namespace opengemini::impl::http {

#if __cplusplus < 202002L
template<typename STREAM>
Connection<STREAM>::Connection(Stream _stream, bool _used) :
    stream(std::move(_stream)),
    used(_used)
{ }
#endif // (__cplusplus < 202002L)

template<typename STREAM>
bool Connection<STREAM>::ShouldRetry(boost::beast::error_code& error,
                                     std::string_view          what) const
{
    if (!error) { return false; }
    if (used) { return true; }
    throw Exception(std::move(error), std::string(what));
}

template<typename DERIVED, typename STREAM>
ConnectionPool<DERIVED, STREAM>::ConnectionPool(
    boost::asio::io_context&  ctx,
    std::chrono::milliseconds connectTimeout,
    std::size_t               maxKeepalive) :
    TaskSlot(ctx),
    connectTimeout_(connectTimeout),
    maxKeepalive_(maxKeepalive)
{ }

template<typename DERIVED, typename STREAM>
typename ConnectionPool<DERIVED, STREAM>::ConnectionPtr
ConnectionPool<DERIVED, STREAM>::Retrieve(const Endpoint&            endpoint,
                                          boost::asio::yield_context yield)
{
    {
        std::lock_guard lock(mutex_);
        if (auto& connections = pool_[endpoint]; !connections.empty()) {
            auto connection = std::move(connections.front());
            connections.pop();
            return connection;
        }
    }

    return static_cast<DERIVED*>(this)->CreateConnection(endpoint, yield);
}

template<typename DERIVED, typename STREAM>
void ConnectionPool<DERIVED, STREAM>::Push(const Endpoint& endpoint,
                                           ConnectionPtr   connection)
{
    if (!connection->used) { connection->used = true; }

    std::lock_guard lock(mutex_);
    auto&           connections = pool_[endpoint];
    if (connections.size() >= maxKeepalive_) { return; }
    pool_[endpoint].push(std::move(connection));
}

} // namespace opengemini::impl::http
