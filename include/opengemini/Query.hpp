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

#ifndef OPENGEMINI_QUERY_HPP
#define OPENGEMINI_QUERY_HPP

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "opengemini/Precision.hpp"

namespace opengemini {

///
/// \~English
/// @brief Holds the query statement.
///
/// \~Chinese
/// @brief 存放查询语句。
///
struct Query {
    ///
    /// \~English
    /// @brief Name of the database.
    ///
    /// \~Chinese
    /// @brief 数据库名称。
    ///
    std::string database;

    ///
    /// \~English
    /// @brief The query command.
    /// @see
    /// <a href="https://docs.opengemini.org/guide/query_data/select.html">
    /// OpenGemini query data doc</a>
    ///
    /// \~Chinese
    /// @brief 查询命令。
    /// @see
    /// <a href="https://docs.opengemini.org/zh/guide/query_data/select.html">
    /// OpenGemini数据查询文档</a>
    ///
    std::string command;

    ///
    /// \~English
    /// @brief Name of the retention policy.
    ///
    /// \~Chinese
    /// @brief 数据保留策略名称。
    ///
    std::string retentionPolicy;

    ///
    /// \~English
    /// @brief Timestamp precision, default to nanosecond.
    ///
    /// \~Chinese
    /// @brief 时间戳精度，默认为纳秒。
    ///
    Precision precision{ Precision::Nanosecond };
};

///
/// \~English
/// @brief Holds the series data.
///
/// \~Chinese
/// @brief 存放时序数据。
///
struct Series {
    using Value = std::
        variant<std::monostate, double, int64_t, uint64_t, std::string, bool>;

    std::string                                  name;
    std::unordered_map<std::string, std::string> tags;
    std::vector<std::string>                     columns;
    std::vector<std::vector<Value>>              values;
};

struct SeriesResult {
    std::vector<Series> series;
    std::string         error;
};

struct QueryResult {
    std::vector<SeriesResult> results;
    std::string               error;
};

} // namespace opengemini

#include "opengemini/impl/Query.ipp"

#endif // !OPENGEMINI_QUERY_HPP
