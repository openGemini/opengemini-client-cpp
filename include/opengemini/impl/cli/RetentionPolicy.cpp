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

#include "opengemini/impl/cli/Functor.hpp"

#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl {

OPENGEMINI_INLINE_SPECIFIER
void ClientImpl::Functor::RunCreateRetentionPolicy::operator()(
    boost::asio::yield_context yield) const
{
    const auto& [name, duration, shardDur, indexDur] = rpConfig_;
    if (db_.empty() || name.empty() || duration.empty()) {
        throw Exception(
            errc::LogicErrors::InvalidArgument,
            "Database name, policy name and duration can not be empty");
    }

    auto cmd =
        fmt::format(CREATE_RP,
                    name,
                    db_,
                    duration,
                    shardDur.empty() ? "" : fmt::format(SHARD_DUR, shardDur),
                    indexDur.empty() ? "" : fmt::format(INDEX_DUR, indexDur),
                    isDefault_ ? DEFAULT : "");

    auto queryResult = RunQueryPost{ impl_, { {}, std::move(cmd) } }(yield);
    if (auto error = free::HasError(queryResult); error) {
        throw Exception(
            errc::ServerErrors::ErrorResult,
            fmt::format("Create retention policy failed: {}", *error));
    }
}

OPENGEMINI_INLINE_SPECIFIER
std::vector<RetentionPolicy>
ClientImpl::Functor::RunShowRetentionPolicies::operator()(
    boost::asio::yield_context yield) const
{
    if (db_.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "Database name can not be empty");
    }

    auto queryResult = RunQueryGet{ impl_, { db_, SHOW_RP } }(yield);
    if (auto error = free::HasError(queryResult); error) {
        throw Exception(
            errc::ServerErrors::ErrorResult,
            fmt::format("Show retention policy failed: {}", *error));
    }

    const auto& result = queryResult.results;
    const auto& series = result.front().series;
    if (result.empty() || series.empty()) { return {}; }

    constexpr auto get = [](const Series::Value& value, auto& field) {
        if (auto pvalue = std::get_if<std::decay_t<decltype(field)>>(&value);
            pvalue) {
            field = *pvalue;
            return true;
        }
        return false;
    };

    std::vector<RetentionPolicy> policies;
    for (const auto& values : series.front().values) {
        // There should be at least 8 columns in the table.
        if (values.size() < 8) { break; }

        if (RetentionPolicy policy; get(values[0], policy.name) &&
                                    get(values[1], policy.duration) &&
                                    get(values[2], policy.shardGroupDuration) &&
                                    get(values[3], policy.hotDuration) &&
                                    get(values[4], policy.warmDuration) &&
                                    get(values[5], policy.indexDuration) &&
                                    get(values[6], policy.replicaNum) &&
                                    get(values[7], policy.isDefault)) {
            policies.push_back(std::move(policy));
        }
    }
    return policies;
}

OPENGEMINI_INLINE_SPECIFIER
void ClientImpl::Functor::RunDropRetentionPolicy::operator()(
    boost::asio::yield_context yield) const
{
    if (db_.empty() || rp_.empty()) {
        throw Exception(
            errc::LogicErrors::InvalidArgument,
            "Database name and retention policy name can not be empty");
    }

    auto queryResult =
        RunQueryPost{ impl_, { {}, fmt::format(DROP_RP, rp_, db_) } }(yield);
    if (auto error = free::HasError(queryResult); error) {
        throw Exception(
            errc::ServerErrors::ErrorResult,
            fmt::format("Drop retention policy failed: {}", *error));
    }
}

} // namespace opengemini::impl
