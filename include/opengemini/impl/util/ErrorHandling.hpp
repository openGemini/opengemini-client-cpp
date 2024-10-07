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

#ifndef OPENGEMINI_IMPL_UTIL_ERRORHANDLING_HPP
#define OPENGEMINI_IMPL_UTIL_ERRORHANDLING_HPP

#include <boost/exception/diagnostic_information.hpp>

#include "opengemini/Exception.hpp"

namespace opengemini::util {

inline void
ConvertError(Error&             error,
             std::exception_ptr exception = std::current_exception())
{
    if (!exception) { return; }

    try {
        std::rethrow_exception(exception);
    }
    catch (const Exception& ex) {
        error = ex.UnderlyingError();
    }
    catch (...) {
        error.Assign(errc::RuntimeErrors::Unexpected,
                     boost::current_exception_diagnostic_information());
    }
}

inline std::exception_ptr
ConvertException(std::exception_ptr exception = std::current_exception())
{
    if (!exception) { return nullptr; }

    try {
        std::rethrow_exception(exception);
    }
    catch (const Exception& ex) {
        return exception;
    }
    catch (...) {
        return std::make_exception_ptr(
            Exception(errc::RuntimeErrors::Unexpected,
                      boost::current_exception_diagnostic_information()));
    }
}

} // namespace opengemini::util

#endif // !OPENGEMINI_IMPL_UTIL_ERRORHANDLING_HPP
