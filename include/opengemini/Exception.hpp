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

#ifndef OPENGEMINI_EXCEPTION_HPP
#define OPENGEMINI_EXCEPTION_HPP

#include <exception>

#include "opengemini/Error.hpp"

namespace opengemini {

///
/// \~English
/// @brief General exception of the library.
///
/// \~Chinese
/// @brief 库通用异常类。
///
class Exception : public std::exception {
public:
    explicit Exception(Error error) :
        error_(std::move(error)),
        what_(error_.What())
    { }
    explicit Exception(std::error_code code, std::string info = {}) :
        error_(code, info),
        what_(error_.What())
    { }
    Exception(const Exception& ex) : error_(ex.error_), what_(ex.what_) { }
    Exception(Exception&& ex) noexcept :
        error_(std::move(ex.error_)),
        what_(std::move(ex.what_))
    { }
    ~Exception() = default;

    ///
    /// \~English
    /// @brief Returns a string object describing the exception.
    ///
    /// \~Chinese
    /// @brief 返回描述异常信息的字符串。
    ///
    const std::string& What() const noexcept { return what_; }

    ///
    /// \~English
    /// @brief Returns the stored error object.
    ///
    /// \~Chinese
    /// @brief 返回承载的错误对象。
    ///
    const Error& UnderlyingError() const noexcept { return error_; }

    ///
    /// \~English
    /// @brief Returns a static string describing the exception。Same as @ref
    /// What(), just for compatibility with std::exception.
    ///
    /// \~Chinese
    /// @brief 返回描述异常信息的字符串。与 @ref What() 相同，仅为满足
    /// std::exception 约束而实现。
    ///
    const char* what() const noexcept override { return what_.c_str(); }

    Exception& operator=(Exception ex) noexcept
    {
        swap(*this, ex);
        return *this;
    }

    friend void swap(Exception& lhs, Exception& rhs) noexcept
    {
        using std::swap;
        swap(lhs.error_, rhs.error_);
        swap(lhs.what_, rhs.what_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Exception& ex)
    {
        os << ex.What();
        return os;
    }

private:
    Error       error_;
    std::string what_;
};

} // namespace opengemini

#endif // !OPENGEMINI_EXCEPTION_HPP
