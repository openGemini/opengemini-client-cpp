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

#ifndef OPENGEMINI_IMPL_CLI_DATABASE_DATABASE_HPP
#define OPENGEMINI_IMPL_CLI_DATABASE_DATABASE_HPP

#include "opengemini/RetentionPolicy.hpp"
#include "opengemini/impl/cli/Functor.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl::cli {

struct RunCreateDatabase : public Functor {
    void operator()(boost::asio::yield_context yield) const;

    std::string             db_;
    std::optional<RpConfig> rpConfig_;

    static constexpr auto CREATE_DB{ R"(CREATE DATABASE "{}")" };
    static constexpr auto CREATE_DB_WITH_RP{
        R"(CREATE DATABASE "{}" WITH{} REPLICATION 1{}{}{})"
    };
    static constexpr auto NAME{ " NAME {}" };
    static constexpr auto DURATION{ " DURATION {}" };
    static constexpr auto SHARD_DUR{ " SHARD DURATION {}" };
    static constexpr auto INDEX_DUR{ " INDEX DURATION {}" };
};

struct RunShowDatabase : public Functor {
    std::vector<std::string> operator()(boost::asio::yield_context yield) const;

    static constexpr auto SHOW_DB{ "SHOW DATABASES" };
};

struct RunDropDatabase : public Functor {
    void operator()(boost::asio::yield_context yield) const;

    std::string db_;

    static constexpr auto DROP_DB{ R"(DROP DATABASE "{}")" };
};

} // namespace opengemini::impl::cli

#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/cli/database/Database.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION

#endif // !OPENGEMINI_IMPL_CLI_DATABASE_DATABASE_HPP
