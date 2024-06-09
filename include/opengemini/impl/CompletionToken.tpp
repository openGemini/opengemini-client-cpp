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

#include "opengemini/CompletionToken.hpp"

#include "opengemini/impl/util/Preprocessor.hpp"

namespace boost::asio {

template<typename SIGNATURE>
class async_result<opengemini::token::Sync, SIGNATURE> {
public:
    template<typename INITIATION,
             typename RAW_COMPLETION_TOKEN,
             typename... ARGS>
    static auto
    initiate(INITIATION&& init, RAW_COMPLETION_TOKEN&&, ARGS&&... args)
    {
        return async_initiate<const use_future_t<>&, SIGNATURE>(
                   [](auto&& handler, auto&& _init, auto&&... _args) {
                       std::move(_init)(OPENGEMINI_PF(handler),
                                        OPENGEMINI_PF(_args)...);
                   },
                   use_future,
                   OPENGEMINI_PF(init),
                   OPENGEMINI_PF(args)...)
            .get();
    }
};

} // namespace boost::asio
