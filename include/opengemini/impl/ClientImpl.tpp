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
