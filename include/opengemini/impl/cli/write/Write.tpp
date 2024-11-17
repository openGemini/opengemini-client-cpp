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

#include "opengemini/impl/cli/write/Write.hpp"

#include <boost/url.hpp>

#include "opengemini/Exception.hpp"
#include "opengemini/impl/comm/UrlTargets.hpp"
#include "opengemini/impl/enc/LineProtocolEncoder.hpp"

namespace opengemini::impl::cli {

template<typename POINT_TYPE>
void RunWrite<POINT_TYPE>::operator()(boost::asio::yield_context yield) const
{
    if (db_.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "Database name cannot be empty");
    }

    auto content = enc::LineProtocolEncoder{}.Encode(point_);
    if (content.empty()) { return; }

    boost::url target(url::WRITE);
    target.set_query(fmt::format("db={}&rp={}", db_, rp_));

    auto rsp = http_.Post(lb_.PickAvailableServer(),
                          target.buffer(),
                          std::move(content),
                          yield);
    if (rsp.result() != http::Status::no_content) {
        throw Exception(errc::ServerErrors::UnexpectedStatusCode,
                        fmt::format("Received code: {}, body:{}",
                                    rsp.result_int(),
                                    rsp.body()));
    }
}

} // namespace opengemini::impl::cli
