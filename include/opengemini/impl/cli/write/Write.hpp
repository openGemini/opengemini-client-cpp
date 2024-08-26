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

#ifndef OPENGEMINI_IMPL_CLI_WRITE_WRITE_HPP
#define OPENGEMINI_IMPL_CLI_WRITE_WRITE_HPP

#include "opengemini/impl/cli/Functor.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl::cli {

template<typename POINT_TYPE>
struct RunWrite : public Functor {
    void operator()(boost::asio::yield_context yield) const;

    std::string db_;
    std::string rp_;
    POINT_TYPE  point_;
};

} // namespace opengemini::impl::cli

#include "opengemini/impl/cli/write/Write.tpp"

#endif // !OPENGEMINI_IMPL_CLI_WRITE_WRITE_HPP
