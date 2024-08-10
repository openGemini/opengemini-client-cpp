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

#ifndef OPENGEMINI_IMPL_CLIENTIMPL_HPP
#define OPENGEMINI_IMPL_CLIENTIMPL_HPP

#include <memory>
#include <type_traits>

#include "opengemini/ClientConfig.hpp"
#include "opengemini/Query.hpp"
#include "opengemini/RetentionPolicy.hpp"
#include "opengemini/impl/comm/Context.hpp"
#include "opengemini/impl/http/IHttpClient.hpp"
#include "opengemini/impl/lb/LoadBalancer.hpp"
#include "opengemini/impl/util/TypeTraits.hpp"

namespace opengemini::impl {

class ClientImpl {
public:
    explicit ClientImpl(const ClientConfig& config);
    ~ClientImpl();

    template<typename COMPLETION_TOKEN>
    auto Ping(std::size_t index, COMPLETION_TOKEN&& token);

    template<typename COMPLETION_TOKEN>
    auto Query(struct Query query, COMPLETION_TOKEN&& token);

    template<typename COMPLETION_TOKEN>
    auto CreateDatabase(std::string_view        database,
                        std::optional<RpConfig> rpConfig,
                        COMPLETION_TOKEN&&      token);

    template<typename COMPLETION_TOKEN>
    auto ShowDatabase(COMPLETION_TOKEN&& token);

    template<typename COMPLETION_TOKEN>
    auto DropDatabase(std::string_view database, COMPLETION_TOKEN&& token);

    template<typename COMPLETION_TOKEN>
    auto CreateRetentionPolicy(std::string_view   database,
                               RpConfig           rpConfig,
                               bool               isDefault,
                               COMPLETION_TOKEN&& token);

    template<typename COMPLETION_TOKEN>
    auto ShowRetentionPolicies(std::string_view   database,
                               COMPLETION_TOKEN&& token);

    template<typename COMPLETION_TOKEN>
    auto DropRetentionPolicy(std::string_view   database,
                             std::string_view   retentionPolicy,
                             COMPLETION_TOKEN&& token);

private:
    std::shared_ptr<http::IHttpClient>
    ConstructHttpClient(const ClientConfig& config);

    template<typename COMPLETION_SIGNATURE,
             typename COMPLETION_TOKEN,
             typename FUNCTION,
             typename = std::enable_if_t<
                 std::is_same_v<util::HandlerExtraArgs_t<COMPLETION_SIGNATURE>,
                                std::tuple<>>>>
    void Spawn(FUNCTION&& func, COMPLETION_TOKEN&& token);

    template<typename COMPLETION_SIGNATURE,
             typename COMPLETION_TOKEN,
             typename FUNCTION,
             typename EXTRA_ARGS = std::enable_if_t<
                 (std::tuple_size_v<
                      util::HandlerExtraArgs_t<COMPLETION_SIGNATURE>> > 0),
                 util::HandlerExtraArgs_t<COMPLETION_SIGNATURE>>,
             typename = void>
    void Spawn(FUNCTION&& func, COMPLETION_TOKEN&& token);

private:
    struct Functor;

private:
    Context                            ctx_;
    std::shared_ptr<http::IHttpClient> http_;
    std::shared_ptr<lb::LoadBalancer>  lb_;
};

} // namespace opengemini::impl

#include "opengemini/impl/ClientImpl.tpp"
#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/ClientImpl.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION

#endif // !OPENGEMINI_IMPL_CLIENTIMPL_HPP
