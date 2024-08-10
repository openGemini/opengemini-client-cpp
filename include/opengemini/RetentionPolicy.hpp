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

#ifndef OPENGEMINI_RETENTIONPOLICY_HPP
#define OPENGEMINI_RETENTIONPOLICY_HPP

#include <string>

#include <fmt/format.h>

namespace opengemini {

///
/// \~English
/// @brief The retention policy configuration.
///
/// \~Chinese
/// @brief 保留策略配置。
///
struct RpConfig {
    ///
    /// \~English
    /// @brief Name of the retention policy.
    ///
    /// \~Chinese
    /// @brief 保留策略的名称。
    ///
    std::string name;

    ///
    /// \~English
    /// @brief Indicates how long OpenGemini keeps the data.
    ///
    /// \~Chinese
    /// @brief 指示OpenGemini保留数据多长时间。
    ///
    std::string duration;

    ///
    /// \~English
    /// @brief Determines the time range of shard group.
    ///
    /// \~Chinese
    /// @brief 决定分片组的时间范围。
    ///
    std::string shardGroupDuration;

    ///
    /// \~English
    /// @brief Determines the time range of index group.
    ///
    /// \~Chinese
    /// @brief 决定索引组的时间范围。
    ///
    std::string indexDuration;
};

///
/// \~English
/// @brief The retention policy.
///
/// \~Chinese
/// @brief 保留策略。
///
struct RetentionPolicy {
    ///
    /// \~English
    /// @brief Name of the retention policy.
    ///
    /// \~Chinese
    /// @brief 保留策略的名称。
    ///
    std::string name;

    ///
    /// \~English
    /// @brief How long OpenGemini keeps the data.
    ///
    /// \~Chinese
    /// @brief OpenGemini保留数据多长时间。
    ///
    std::string duration;

    ///
    /// \~English
    /// @brief The time range of shard group.
    ///
    /// \~Chinese
    /// @brief 分片组的时间范围。
    ///
    std::string shardGroupDuration;

    std::string hotDuration;

    std::string warmDuration;

    ///
    /// \~English
    /// @brief The time range of index group.
    ///
    /// \~Chinese
    /// @brief 索引组的时间范围。
    ///
    std::string indexDuration;

    uint64_t replicaNum;

    ///
    /// \~English
    /// @brief Whether it is the default policy.
    ///
    /// \~Chinese
    /// @brief 是否为默认策略。
    ///
    bool isDefault;

    friend bool operator==(const RetentionPolicy& lhs,
                           const RetentionPolicy& rhs) noexcept
    {
        return (lhs.duration == rhs.duration) &&
               (lhs.isDefault == rhs.isDefault) &&
               (lhs.replicaNum == rhs.replicaNum) &&
               (lhs.hotDuration == rhs.hotDuration) &&
               (lhs.warmDuration == rhs.warmDuration) &&
               (lhs.indexDuration == rhs.indexDuration) &&
               (lhs.shardGroupDuration == rhs.shardGroupDuration) &&
               (lhs.name == rhs.name);
    }
};

} // namespace opengemini

// We include this because anyone who uses RpConfig
// will very likely need the literals.
#include "opengemini/impl/util/DurationLiterals.hpp"

#endif // !OPENGEMINI_RETENTIONPOLICY_HPP
