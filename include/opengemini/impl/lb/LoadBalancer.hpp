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

#ifndef OPENGEMINI_IMPL_LB_LOADBALANCER_HPP
#define OPENGEMINI_IMPL_LB_LOADBALANCER_HPP

#include <atomic>
#include <memory>
#include <vector>

#include "opengemini/Endpoint.hpp"
#include "opengemini/impl/comm/TaskSlot.hpp"
#include "opengemini/impl/http/IHttpClient.hpp"

namespace opengemini::impl::lb {

class LoadBalancer :
    public TaskSlot,
    public std::enable_shared_from_this<LoadBalancer> {
private:
    struct PrivateConstructor {
        constexpr PrivateConstructor() = default;
    };

public:
    template<typename... ARGS>
    static std::shared_ptr<LoadBalancer> Construct(ARGS&&... args)
    {
        return std::make_shared<LoadBalancer>(PrivateConstructor{},
                                              std::forward<ARGS>(args)...);
    }

    LoadBalancer(
        PrivateConstructor,
        boost::asio::io_context&           ctx,
        const std::vector<Endpoint>&       endpoints,
        std::shared_ptr<http::IHttpClient> http,
        std::chrono::milliseconds healthCheckPeriod = std::chrono::seconds(10));

    ~LoadBalancer() = default;

    void StartHealthCheck();
    void StopHealthCheck();

    const Endpoint& PickServer(std::size_t index) const;
    const Endpoint& PickAvailableServer();

private:
    struct Server {
        Endpoint          endpoint;
        std::atomic<bool> good{ true };
    };

private:
    void HealthCheck(boost::asio::yield_context yield);

    std::vector<Server>
    ConstructServers(const std::vector<Endpoint>& endpoints) const;

private:
    std::shared_ptr<http::IHttpClient> http_;
    std::vector<Server>                servers_;
    std::atomic<std::size_t>           nextIdx_{ 0 };

    boost::asio::steady_timer timer_;

    const std::chrono::milliseconds healthCheckPeriod_;
};

} // namespace opengemini::impl::lb

#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/lb/LoadBalancer.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION

#endif // !OPENGEMINI_IMPL_LB_LOADBALANCER_HPP
