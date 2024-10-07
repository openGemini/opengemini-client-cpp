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

#ifndef OPENGEMINI_IMPL_CLI_FUNCTOR_HPP
#define OPENGEMINI_IMPL_CLI_FUNCTOR_HPP

#include "opengemini/impl/ClientImpl.hpp"

namespace opengemini::impl {

struct ClientImpl::Functor {
    struct RunPing {
        ClientImpl* impl_;
        std::size_t index_;

        std::string operator()(boost::asio::yield_context yield) const;
    };

    struct RunCreateDatabase {
        ClientImpl*             impl_;
        std::string             db_;
        std::optional<RpConfig> rpConfig_;

        static constexpr auto CREATE_DB{ R"(CREATE DATABASE "{}")" };
        static constexpr auto CREATE_DB_WITH_RP{
            R"(CREATE DATABASE "{}" WITH{} REPLICATION 1{}{}{})"
        };
        static constexpr auto NAME{ " NAME {}" };

        void operator()(boost::asio::yield_context yield) const;
    };

    struct RunShowDatabase {
        ClientImpl* impl_;

        static constexpr auto SHOW_DB{ "SHOW DATABASES" };

        std::vector<std::string>
        operator()(boost::asio::yield_context yield) const;
    };

    struct RunDropDatabase {
        ClientImpl* impl_;
        std::string db_;

        static constexpr auto DROP_DB{ R"(DROP DATABASE "{}")" };

        void operator()(boost::asio::yield_context yield) const;
    };

    struct RunCreateRetentionPolicy {
        ClientImpl* impl_;
        std::string db_;
        RpConfig    rpConfig_;
        bool        isDefault_;

        static constexpr auto CREATE_RP{
            R"(CREATE RETENTION POLICY {} ON "{}" DURATION {} REPLICATION 1{}{}{})"
        };
        static constexpr auto DEFAULT{ " DEFAULT" };

        void operator()(boost::asio::yield_context yield) const;
    };

    struct RunShowRetentionPolicies {
        ClientImpl* impl_;
        std::string db_;

        static constexpr auto SHOW_RP{ "SHOW RETENTION POLICIES" };

        std::vector<RetentionPolicy>
        operator()(boost::asio::yield_context yield) const;
    };

    struct RunDropRetentionPolicy {
        ClientImpl* impl_;
        std::string db_;
        std::string rp_;

        static constexpr auto DROP_RP{ R"(DROP RETENTION POLICY {} ON "{}")" };

        void operator()(boost::asio::yield_context yield) const;
    };

    template<typename POINT_TYPE>
    struct RunWrite {
        ClientImpl* impl_;
        std::string db_;
        std::string rp_;
        POINT_TYPE  point_;

        void operator()(boost::asio::yield_context yield) const;
    };

    struct RunQueryGet {
        ClientImpl*  impl_;
        struct Query query_;

        QueryResult operator()(boost::asio::yield_context yield) const;
    };

    struct RunQueryPost {
        ClientImpl*  impl_;
        struct Query query_;

        QueryResult operator()(boost::asio::yield_context yield) const;
    };

    static constexpr auto DURATION{ " DURATION {}" };
    static constexpr auto SHARD_DUR{ " SHARD DURATION {}" };
    static constexpr auto INDEX_DUR{ " INDEX DURATION {}" };
};

} // namespace opengemini::impl

#endif // !OPENGEMINI_IMPL_CLI_FUNCTOR_HPP
