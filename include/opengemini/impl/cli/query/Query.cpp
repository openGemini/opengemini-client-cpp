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

#include "opengemini/impl/cli/query/Query.hpp"

#include <boost/url.hpp>

#include "opengemini/Exception.hpp"
#include "opengemini/impl/comm/UrlTargets.hpp"

namespace opengemini::impl::cli {

namespace {

inline void CheckQuery(const struct Query& query)
{
    if (query.command.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "Field [command] must not be empty");
    }
}

inline auto ParseQueryRsp(const http::Response& rsp)
{
    if (rsp.result() != http::Status::ok) {
        throw Exception(errc::ServerErrors::UnexpectedStatusCode,
                        fmt::format("Received code: {}, body:{}",
                                    rsp.result_int(),
                                    rsp.body()));
    }

    return nlohmann::json::parse(rsp.body()).get<QueryResult>();
}

} // namespace

OPENGEMINI_INLINE_SPECIFIER
QueryResult RunQueryGet::operator()(boost::asio::yield_context yield) const
{
    CheckQuery(query_);

    boost::url target(url::QUERY);
    target.set_query(fmt::format("db={}&q={}&rp={}&epoch={}",
                                 query_.database,
                                 query_.command,
                                 query_.retentionPolicy,
                                 ToString(query_.precision)));

    return ParseQueryRsp(
        http_.Get(lb_.PickAvailableServer(), target.buffer(), yield));
}

OPENGEMINI_INLINE_SPECIFIER
QueryResult RunQueryPost::operator()(boost::asio::yield_context yield) const
{
    CheckQuery(query_);

    boost::url target(url::QUERY);
    target.set_query(
        fmt::format("db={}&q={}", query_.database, query_.command));

    return ParseQueryRsp(
        http_.Post(lb_.PickAvailableServer(), target.buffer(), {}, yield));
}

} // namespace opengemini::impl::cli
