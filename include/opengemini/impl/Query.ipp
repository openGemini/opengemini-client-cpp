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

#include "opengemini/Query.hpp"

#include <ostream>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

namespace nlohmann {

template<>
struct adl_serializer<opengemini::Series::Value> {
    static void to_json(json& _json, const opengemini::Series::Value& value)
    {
        _json = value;
    }

    static void from_json(const json& _json, opengemini::Series::Value& value)
    {
        using vt = detail::value_t;
        switch (_json.type()) {
        case vt::number_float: value = _json.get<double>(); break;
        case vt::number_integer: value = _json.get<int64_t>(); break;
        case vt::number_unsigned: value = _json.get<uint64_t>(); break;
        case vt::string: value = _json.get<std::string>(); break;
        case vt::boolean: value = _json.get<bool>(); break;
        default: value = {};
        }
    }
};

} // namespace nlohmann

namespace fmt {

template<>
struct formatter<opengemini::Series::Value> : formatter<std::string> {
    auto format(const opengemini::Series::Value& value,
                format_context&                  ctx) const
    {
        return formatter<std::string>::format(
            std::visit(
                [](const auto& alter) -> std::string {
                    if constexpr (std::is_same_v<std::decay_t<decltype(alter)>,
                                                 std::monostate>) {
                        return "<null>";
                    }
                    else {
                        return fmt::format("{}", alter);
                    }
                },
                value),
            ctx);
    }
};

} // namespace fmt

namespace opengemini {

// clang-format off
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Series, name, tags, columns, values)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(SeriesResult, series, error)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(QueryResult, results, error)
// clang-format on

} // namespace opengemini

namespace opengemini {

inline std::ostream& operator<<(std::ostream& os, const Series& series)
{
    os << fmt::format("Series name: {}\n", series.name)
       << fmt::format("Series tags: {}\n", series.tags)
       << fmt::format("{:^20}\n", fmt::join(series.columns, "|"))
       << fmt::format("{:-^{}}\n", "", series.columns.size() * 20);
    for (auto& row : series.values) {
        os << fmt::format("{:^20}\n", fmt::join(row, "|"));
    }

    return os;
}

inline std::ostream& operator<<(std::ostream&       os,
                                const SeriesResult& seriesResult)
{
    if (!seriesResult.error.empty()) {
        os << fmt::format("Series result error: {}\n", seriesResult.error);
        if (seriesResult.series.empty()) { return os; }
    }

    for (auto& series : seriesResult.series) { os << series; }

    return os;
}

inline std::ostream& operator<<(std::ostream&      os,
                                const QueryResult& queryResult)
{
    if (!queryResult.error.empty()) {
        os << fmt::format("Query result error: {}\n", queryResult.error);
        if (queryResult.results.empty()) { return os; }
    }

    for (auto& result : queryResult.results) { os << result; }

    return os;
}

} // namespace opengemini

namespace opengemini::impl::free {

inline const std::string* HasError(const QueryResult& queryResult)
{
    if (!queryResult.error.empty()) { return &(queryResult.error); }

    for (auto& result : queryResult.results) {
        if (!result.error.empty()) { return &result.error; }
    }

    return nullptr;
}

} // namespace opengemini::impl::free
