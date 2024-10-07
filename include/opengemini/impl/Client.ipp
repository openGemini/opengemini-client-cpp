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

#include "opengemini/Client.hpp"

#include <cassert>

#include "opengemini/impl/ClientImpl.hpp"

namespace opengemini {

inline Client::Client(const ClientConfig& config) :
    impl_(std::make_unique<impl::ClientImpl>(config))
{ }

inline Client::Client(Client&& client) noexcept : impl_(std::move(client.impl_))
{ }

inline Client& Client::operator=(Client&& client) noexcept
{
    assert(this != &client);
    impl_ = std::move(client.impl_);
    return *this;
}

template<typename COMPLETION_TOKEN>
auto Client::Ping(std::size_t index, COMPLETION_TOKEN&& token)
{
    return impl_->Ping(index, std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::Query(struct Query query, COMPLETION_TOKEN&& token)
{
    return impl_->Query(std::move(query),
                        std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::CreateDatabase(std::string_view        database,
                            std::optional<RpConfig> rpConfig,
                            COMPLETION_TOKEN&&      token)
{
    return impl_->CreateDatabase(database,
                                 std::move(rpConfig),
                                 std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::ShowDatabase(COMPLETION_TOKEN&& token)
{
    return impl_->ShowDatabase(std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::DropDatabase(std::string_view database, COMPLETION_TOKEN&& token)
{
    return impl_->DropDatabase(database, std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::CreateRetentionPolicy(std::string_view   database,
                                   RpConfig           rpConfig,
                                   bool               isDefault,
                                   COMPLETION_TOKEN&& token)
{
    return impl_->CreateRetentionPolicy(database,
                                        std::move(rpConfig),
                                        isDefault,
                                        std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::ShowRetentionPolicies(std::string_view   database,
                                   COMPLETION_TOKEN&& token)
{
    return impl_->ShowRetentionPolicies(database,
                                        std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::DropRetentionPolicy(std::string_view   database,
                                 std::string_view   retentionPolicy,
                                 COMPLETION_TOKEN&& token)
{
    return impl_->DropRetentionPolicy(database,
                                      retentionPolicy,
                                      std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::Write(std::string_view   database,
                   Point              point,
                   std::string_view   retentionPolicy,
                   COMPLETION_TOKEN&& token)
{
    return impl_->Write<Point>(database,
                               std::move(point),
                               retentionPolicy,
                               std::forward<COMPLETION_TOKEN>(token));
}

template<typename COMPLETION_TOKEN>
auto Client::Write(std::string_view   database,
                   std::vector<Point> points,
                   std::string_view   retentionPolicy,
                   COMPLETION_TOKEN&& token)
{
    return impl_->Write<std::vector<Point>>(
        database,
        std::move(points),
        retentionPolicy,
        std::forward<COMPLETION_TOKEN>(token));
}

} // namespace opengemini
