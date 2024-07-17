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

#include "opengemini/impl/cli/Functor.hpp"

#include <boost/url.hpp>

#include "opengemini/impl/comm/UrlTargets.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl {

OPENGEMINI_INLINE_SPECIFIER
QueryResult ClientImpl::Functor::RunQueryGet::operator()(
    boost::asio::yield_context yield) const
{
    if (query_.command.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "Field [command] must not be empty");
    }

    boost::url target(url::QUERY);
    target.set_query(fmt::format("db={}&q={}&rp={}&epoch={}",
                                 query_.database,
                                 query_.command,
                                 query_.retentionPolicy,
                                 ToString(query_.precision)));

    auto rsp = impl_->http_->Get(impl_->lb_->PickAvailableServer(),
                                 target.buffer(),
                                 yield);

    if (rsp.result() != http::Status::ok) {
        throw Exception(errc::ServerErrors::UnexpectedStatusCode,
                        fmt::format("Received code: {}, body:{}",
                                    rsp.result_int(),
                                    rsp.body()));
    }

    return nlohmann::json::parse(rsp.body()).get<QueryResult>();
}

} // namespace opengemini::impl
