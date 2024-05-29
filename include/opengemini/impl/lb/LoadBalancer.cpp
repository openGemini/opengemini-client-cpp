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

#include "opengemini/impl/lb/LoadBalancer.hpp"

#include <unordered_set>

#include <fmt/format.h>

#include "opengemini/Exception.hpp"
#include "opengemini/impl/comm/UrlTargets.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl::lb {

OPENGEMINI_INLINE_SPECIFIER
LoadBalancer::LoadBalancer(PrivateConstructor,
                           boost::asio::io_context&           ctx,
                           const std::vector<Endpoint>&       endpoints,
                           std::shared_ptr<http::IHttpClient> http,
                           std::chrono::milliseconds healthCheckPeriod) :
    TaskSlot(ctx),
    http_(http),
    servers_(ConstructServers(endpoints)),
    timer_(ctx_, healthCheckPeriod),
    healthCheckPeriod_(healthCheckPeriod)
{ }

OPENGEMINI_INLINE_SPECIFIER
void LoadBalancer::StartHealthCheck()
{
    boost::asio::spawn(
        ctx_,
        [self = shared_from_this(), this](auto yield) { HealthCheck(yield); },
        boost::asio::detached);
}

OPENGEMINI_INLINE_SPECIFIER
void LoadBalancer::StopHealthCheck()
{
    timer_.cancel();
}

OPENGEMINI_INLINE_SPECIFIER
const Endpoint& LoadBalancer::PickServer(std::size_t index) const
{
    if (index >= servers_.size()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "Index out of range");
    }

    return servers_[index].endpoint;
}

OPENGEMINI_INLINE_SPECIFIER
const Endpoint& LoadBalancer::PickAvailableServer()
{
    for (std::size_t cnt = 0, size = servers_.size(); cnt < size; ++cnt) {
        auto idx = nextIdx_.fetch_add(1, std::memory_order_relaxed);
        const auto& [endpoint, good] = servers_[idx % size];
        if (!good.load(std::memory_order_relaxed)) { continue; }

        std::size_t newVal{ 0 };
        do {
            newVal = idx % size;
        } while (!nextIdx_.compare_exchange_weak(idx,
                                                 newVal,
                                                 std::memory_order_relaxed));

        return endpoint;
    }

    throw Exception(errc::ServerErrors::NoAvailableServer);
}

OPENGEMINI_INLINE_SPECIFIER
void LoadBalancer::HealthCheck(boost::asio::yield_context yield)
{
    for (Error error;;) {
        timer_.expires_after(healthCheckPeriod_);
        timer_.async_wait(yield);

        for (auto& server : servers_) {
            auto rsp = http_->Get(server.endpoint, url::PING, yield, error);
            server.good.store(!error &&
                                  rsp.result() == http::Status::no_content,
                              std::memory_order_relaxed);
        }
    }
}

OPENGEMINI_INLINE_SPECIFIER
std::vector<LoadBalancer::Server>
LoadBalancer::ConstructServers(const std::vector<Endpoint>& endpoints) const
{
    if (endpoints.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "At least one endpoint needed");
    }

    std::unordered_set<Endpoint, Endpoint::Hasher> dup;
    for (auto& endpoint : endpoints) {
        if (endpoint.host.empty()) {
            throw Exception(errc::LogicErrors::InvalidArgument,
                            "Host field should not be empty");
        }

        auto [it, success] = dup.insert(endpoint);
        if (!success) {
            throw Exception(
                errc::LogicErrors::InvalidArgument,
                fmt::format("Duplicate endpoint: {}:{}", it->host, it->port));
        }
    }

    std::vector<LoadBalancer::Server> servers(endpoints.size());
    for (std::size_t idx = 0; idx < servers.size(); ++idx) {
        servers[idx].endpoint = endpoints[idx];
    }
    return servers;
}

} // namespace opengemini::impl::lb
