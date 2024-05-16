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

#include "opengemini/impl/ErrorCode.hpp"

#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::errc {

std::error_code make_error_code(LogicErrors error)
{
    return std::error_code(static_cast<int>(error),
                           impl::LogicCategory::Instance());
}

} // namespace opengemini::errc

namespace opengemini::errc::impl {

OPENGEMINI_INLINE_SPECIFIER
const LogicCategory& LogicCategory::Instance()
{
    static LogicCategory instance;
    return instance;
}

OPENGEMINI_INLINE_SPECIFIER
const char* LogicCategory::name() const noexcept
{
    return "opengemini.logic";
}

OPENGEMINI_INLINE_SPECIFIER
std::string LogicCategory::message(int value) const
{
    switch (static_cast<LogicErrors>(value)) {
    case LogicErrors::NotImplemented: return "Not implemented";
    case LogicErrors::InvalidArgument: return "Invalid argument";
    }
}

} // namespace opengemini::errc::impl
