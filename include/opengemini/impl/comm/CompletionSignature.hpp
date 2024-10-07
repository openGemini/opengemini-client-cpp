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

#ifndef OPENGEMINI_IMPL_COMM_COMPLETIONSIGNATURE_HPP
#define OPENGEMINI_IMPL_COMM_COMPLETIONSIGNATURE_HPP

#include <exception>
#include <string>
#include <vector>

#include "opengemini/Query.hpp"
#include "opengemini/RetentionPolicy.hpp"

namespace opengemini::impl::sig {

using Ping  = void(std::exception_ptr, std::string);
using Query = void(std::exception_ptr, QueryResult);

using CreateDatabase = void(std::exception_ptr);
using ShowDatabase   = void(std::exception_ptr, std::vector<std::string>);
using DropDatabase   = void(std::exception_ptr);

using CreateRetentionPolicy = void(std::exception_ptr);
using ShowRetentionPolicies = void(std::exception_ptr,
                                   std::vector<RetentionPolicy>);
using DropRetentionPolicy   = void(std::exception_ptr);

using Write = void(std::exception_ptr);

} // namespace opengemini::impl::sig

#endif // !OPENGEMINI_IMPL_COMM_COMPLETIONSIGNATURE_HPP
