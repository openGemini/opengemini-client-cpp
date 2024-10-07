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

#include "opengemini/impl/comm/UrlTargets.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl {

OPENGEMINI_INLINE_SPECIFIER
std::string
ClientImpl::Functor::RunPing::operator()(boost::asio::yield_context yield) const
{
    auto rsp =
        impl_->http_->Get(impl_->lb_->PickServer(index_), url::PING, yield);

    if (rsp.result() != http::Status::no_content) {
        throw Exception(errc::ServerErrors::UnexpectedStatusCode,
                        fmt::format("Received code: {}, body:{}",
                                    rsp.result_int(),
                                    rsp.body()));
    }

    auto version = rsp.find("X-Geminidb-Version");
    return version != rsp.end() ? version->value() : "Unknown";
}

} // namespace opengemini::impl
