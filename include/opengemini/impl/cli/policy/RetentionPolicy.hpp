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

#ifndef OPENGEMINI_IMPL_CLI_POLICY_RETENTIONPOLICY_HPP
#define OPENGEMINI_IMPL_CLI_POLICY_RETENTIONPOLICY_HPP

#include "opengemini/RetentionPolicy.hpp"
#include "opengemini/impl/cli/Functor.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl::cli {

struct RunCreateRetentionPolicy : public Functor {
    void operator()(boost::asio::yield_context yield) const;

    std::string db_;
    RpConfig    rpConfig_;
    bool        isDefault_;

    static constexpr auto CREATE_RP{
        R"(CREATE RETENTION POLICY {} ON "{}" DURATION {} REPLICATION 1{}{}{})"
    };
    static constexpr auto SHARD_DUR{ " SHARD DURATION {}" };
    static constexpr auto INDEX_DUR{ " INDEX DURATION {}" };
    static constexpr auto DEFAULT{ " DEFAULT" };
};

struct RunShowRetentionPolicies : public Functor {
    std::vector<RetentionPolicy>
    operator()(boost::asio::yield_context yield) const;

    std::string db_;

    static constexpr auto SHOW_RP{ "SHOW RETENTION POLICIES" };
};

struct RunDropRetentionPolicy : public Functor {
    void operator()(boost::asio::yield_context yield) const;

    std::string db_;
    std::string rp_;

    static constexpr auto DROP_RP{ R"(DROP RETENTION POLICY {} ON "{}")" };
};

} // namespace opengemini::impl::cli

#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/cli/policy/RetentionPolicy.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION

#endif // !OPENGEMINI_IMPL_CLI_POLICY_RETENTIONPOLICY_HPP
