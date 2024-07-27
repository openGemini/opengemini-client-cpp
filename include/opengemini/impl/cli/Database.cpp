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

#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl {

OPENGEMINI_INLINE_SPECIFIER
void ClientImpl::Functor::RunCreateDatabase::operator()(
    boost::asio::yield_context yield) const
{
    if (db_.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "Database name can not be empty");
    }

    std::string cmd;
    if (!rpConfig_.has_value()) { cmd = fmt::format(CREATE_DB, db_); }
    else {
        const auto& [name, duration, shardDur, indexDur] = rpConfig_.value();

        cmd = fmt::format(
            CREATE_DB_WITH_RP,
            db_,
            duration.empty() ? "" : fmt::format(DURATION, duration),
            shardDur.empty() ? "" : fmt::format(SHARD_DUR, shardDur),
            indexDur.empty() ? "" : fmt::format(INDEX_DUR, indexDur),
            name.empty() ? "" : fmt::format(NAME, name));
    }

    auto queryResult = RunQueryPost{ impl_, { {}, std::move(cmd) } }(yield);
    if (auto error = free::HasError(queryResult); error) {
        throw Exception(errc::ServerErrors::ErrorResult,
                        fmt::format("Create database failed: {}", *error));
    }
}

OPENGEMINI_INLINE_SPECIFIER
std::vector<std::string> ClientImpl::Functor::RunShowDatabase::operator()(
    boost::asio::yield_context yield) const
{
    auto queryResult = RunQueryGet{ impl_, { {}, SHOW_DB } }(yield);
    if (auto error = free::HasError(queryResult); error) {
        throw Exception(errc::ServerErrors::ErrorResult,
                        fmt::format("Show database failed: {}", *error));
    }

    if (queryResult.results.empty() ||
        queryResult.results.front().series.empty()) {
        return {};
    }

    std::vector<std::string> dbs;
    for (const auto& value :
         queryResult.results.front().series.front().values) {
        if (value.empty()) { continue; }

        if (auto db = std::get_if<std::string>(&value.front()); db) {
            dbs.push_back(*db);
        }
    }
    return dbs;
}

OPENGEMINI_INLINE_SPECIFIER
void ClientImpl::Functor::RunDropDatabase::operator()(
    boost::asio::yield_context yield) const
{
    if (db_.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "Database name can not be empty");
    }

    auto queryResult =
        RunQueryGet{ impl_, { {}, fmt::format(DROP_DB, db_) } }(yield);
    if (auto error = free::HasError(queryResult); error) {
        throw Exception(errc::ServerErrors::ErrorResult,
                        fmt::format("Drop database failed: {}", *error));
    }
}

} // namespace opengemini::impl
