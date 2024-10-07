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

#include "opengemini/impl/ClientImpl.hpp"

#include <boost/exception/diagnostic_information.hpp>

#include "opengemini/impl/cli/Functor.hpp"
#include "opengemini/impl/comm/CompletionSignature.hpp"
#include "opengemini/impl/util/ErrorHandling.hpp"
#include "opengemini/impl/util/TypeTraits.hpp"

namespace opengemini::impl {

template<typename COMPLETION_TOKEN>
auto ClientImpl::Ping(std::size_t index, COMPLETION_TOKEN&& token)
{
    using Signature = sig::Ping;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&& token, std::size_t index) {
            static_assert(util::IsInvocable_v<decltype(token), Signature>,
                          "Completion signature of Ping must be: "
                          "void(std::exception_ptr, std::string)");

            Spawn<Signature>(Functor::RunPing{ this, index },
                             OPENGEMINI_PF(token));
        },
        token,
        index);
}

template<typename COMPLETION_TOKEN>
auto ClientImpl::Query(struct Query query, COMPLETION_TOKEN&& token)
{
    using Signature = sig::Query;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&& token, struct Query query) {
            static_assert(util::IsInvocable_v<decltype(token), Signature>,
                          "Completion signature of Query must be: "
                          "void(std::exception_ptr, QueryResult)");

            Spawn<Signature>(Functor::RunQueryGet{ this, std::move(query) },
                             OPENGEMINI_PF(token));
        },
        token,
        std::move(query));
}

template<typename COMPLETION_TOKEN>
auto ClientImpl::CreateDatabase(std::string_view        database,
                                std::optional<RpConfig> rpConfig,
                                COMPLETION_TOKEN&&      token)
{
    using Signature = sig::CreateDatabase;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&&                  token,
               std::string             database,
               std::optional<RpConfig> rpConfig) {
            static_assert(util::IsInvocable_v<decltype(token), Signature>,
                          "Completion signature of CreateDatabase must be: "
                          "void(std::exception_ptr)");

            Spawn<Signature>(Functor::RunCreateDatabase{ this,
                                                         std::move(database),
                                                         std::move(rpConfig) },
                             OPENGEMINI_PF(token));
        },
        token,
        std::string(database),
        std::move(rpConfig));
}

template<typename COMPLETION_TOKEN>
auto ClientImpl::ShowDatabase(COMPLETION_TOKEN&& token)
{
    using Signature = sig::ShowDatabase;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&& token) {
            static_assert(util::IsInvocable_v<decltype(token), Signature>,
                          "Completion signature of ShowDatabase must be: "
                          "void(std::exception_ptr, std::vector<std::string>)");

            Spawn<Signature>(Functor::RunShowDatabase{ this },
                             OPENGEMINI_PF(token));
        },
        token);
}

template<typename COMPLETION_TOKEN>
auto ClientImpl::DropDatabase(std::string_view   database,
                              COMPLETION_TOKEN&& token)
{
    using Signature = sig::DropDatabase;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&& token, std::string database) {
            static_assert(util::IsInvocable_v<decltype(token), Signature>,
                          "Completion signature of DropDatabase must be: "
                          "void(std::exception_ptr)");

            Spawn<Signature>(
                Functor::RunDropDatabase{ this, std::move(database) },
                OPENGEMINI_PF(token));
        },
        token,
        std::string(database));
}

template<typename COMPLETION_TOKEN>
auto ClientImpl::CreateRetentionPolicy(std::string_view   database,
                                       RpConfig           rpConfig,
                                       bool               isDefault,
                                       COMPLETION_TOKEN&& token)
{
    using Signature = sig::CreateRetentionPolicy;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&&      token,
               std::string database,
               RpConfig    rpConfig,
               bool        isDefault) {
            static_assert(
                util::IsInvocable_v<decltype(token), Signature>,
                "Completion signature of CreateRetentionPolicy must be: "
                "void(std::exception_ptr)");

            Spawn<Signature>(
                Functor::RunCreateRetentionPolicy{ this,
                                                   std::move(database),
                                                   std::move(rpConfig),
                                                   isDefault },
                OPENGEMINI_PF(token));
        },
        token,
        std::string(database),
        std::move(rpConfig),
        isDefault);
}

template<typename COMPLETION_TOKEN>
auto ClientImpl::ShowRetentionPolicies(std::string_view   database,
                                       COMPLETION_TOKEN&& token)
{
    using Signature = sig::ShowRetentionPolicies;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&& token, std::string database) {
            static_assert(
                util::IsInvocable_v<decltype(token), Signature>,
                "Completion signature of ShowRetentionPolicies must be: "
                "void(std::exception_ptr, std::vector<RetentionPolicy>)");

            Spawn<Signature>(
                Functor::RunShowRetentionPolicies{ this, std::move(database) },
                OPENGEMINI_PF(token));
        },
        token,
        std::string(database));
}

template<typename COMPLETION_TOKEN>
auto ClientImpl::DropRetentionPolicy(std::string_view   database,
                                     std::string_view   retentionPolicy,
                                     COMPLETION_TOKEN&& token)
{
    using Signature = sig::DropRetentionPolicy;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&&      token,
               std::string database,
               std::string retentionPolicy) {
            static_assert(
                util::IsInvocable_v<decltype(token), Signature>,
                "Completion signature of DropRetentionPolicy must be: "
                "void(std::exception_ptr)");

            Spawn<Signature>(
                Functor::RunDropRetentionPolicy{ this,
                                                 std::move(database),
                                                 std::move(retentionPolicy) },
                OPENGEMINI_PF(token));
        },
        token,
        std::string(database),
        std::string(retentionPolicy));
}

template<typename POINT_TYPE, typename COMPLETION_TOKEN>
auto ClientImpl::Write(std::string_view   database,
                       POINT_TYPE         point,
                       std::string_view   retentionPolicy,
                       COMPLETION_TOKEN&& token)
{
    using Signature = sig::Write;
    return boost::asio::async_initiate<COMPLETION_TOKEN, Signature>(
        [this](auto&&      token,
               std::string database,
               std::string retentionPolicy,
               POINT_TYPE  point) {
            static_assert(util::IsInvocable_v<decltype(token), Signature>,
                          "Completion signature of Write must be: "
                          "void(std::exception_ptr)");

            Spawn<Signature>(
                Functor::RunWrite<POINT_TYPE>{ this,
                                               std::move(database),
                                               std::move(retentionPolicy),
                                               std::move(point) },
                OPENGEMINI_PF(token));
        },
        token,
        std::string(database),
        std::string(retentionPolicy),
        std::move(point));
}

template<typename COMPLETION_SIGNATURE,
         typename COMPLETION_TOKEN,
         typename FUNCTION,
         typename>
void ClientImpl::Spawn(FUNCTION&& func, COMPLETION_TOKEN&& token)
{
    boost::asio::spawn(ctx_(),
                       std::forward<FUNCTION>(func),
                       [_token = std::forward<COMPLETION_TOKEN>(token)](
                           std::exception_ptr ex) mutable {
                           _token(util::ConvertException(ex));
                       });
}

template<typename COMPLETION_SIGNATURE,
         typename COMPLETION_TOKEN,
         typename FUNCTION,
         typename EXTRA_ARGS,
         typename>
void ClientImpl::Spawn(FUNCTION&& func, COMPLETION_TOKEN&& token)
{
    boost::asio::spawn(
        ctx_(),
        std::forward<FUNCTION>(func),
        [_token = std::forward<COMPLETION_TOKEN>(
             token)](std::exception_ptr ex, EXTRA_ARGS args) mutable {
            std::apply(
                _token,
                std::tuple_cat(std::make_tuple(util::ConvertException(ex)),
                               std::move(args)));
        });
}

} // namespace opengemini::impl

#include "opengemini/impl/cli/Write.tpp"
