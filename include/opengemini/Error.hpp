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

#ifndef OPENGEMINI_ERROR_HPP
#define OPENGEMINI_ERROR_HPP

#include <ostream>
#include <system_error>

#include <fmt/format.h>

namespace opengemini {

///
/// \~English
/// @brief General error of the library.
///
/// \~Chinese
/// @brief 库通用错误。
///
class Error {
public:
    Error()  = default;
    ~Error() = default;

    Error(std::error_code code, std::string info = {}) :
        code_(std::move(code)),
        info_(std::move(info))
    { }

    Error(const Error& error) : code_(error.code_), info_(error.info_) { }

    Error(Error&& error) noexcept :
        code_(std::move(error.code_)),
        info_(std::move(error.info_))
    { }

    Error& operator=(Error error) noexcept
    {
        swap(*this, error);
        return *this;
    }

    ///
    /// \~English
    /// @brief Checks if the error code value is valid.
    ///
    /// \~Chinese
    /// @brief 检查错误码是否有效。
    ///
    explicit operator bool() const noexcept { return bool(code_); }

    ///
    /// \~English
    /// @brief Returns the diagnostic information about the error.
    ///
    /// \~Chinese
    /// @brief 返回有关错误的诊断信息。
    ///
    std::string What() const
    {
        return fmt::format("[{}:{}({})] {}",
                           code_.category().name(),
                           code_.value(),
                           code_.message(),
                           info_);
    }

    ///
    /// \~English
    /// @brief Returns the underlying error code.
    ///
    /// \~Chinese
    /// @brief 返回错误码。
    ///
    const std::error_code& Code() const noexcept { return code_; }

    void Clear() noexcept
    {
        code_.clear();
        info_.clear();
    }

    void Assign(const std::error_code& code, std::string info = {}) noexcept
    {
        code_.assign(code.value(), code.category());
        info_ = std::move(info);
    }

    friend void swap(Error& lhs, Error& rhs) noexcept
    {
        using std::swap;
        swap(lhs.code_, rhs.code_);
        swap(lhs.info_, rhs.info_);
    }

    friend bool operator==(const Error& lhs, const Error& rhs) noexcept
    {
        return lhs.code_ == rhs.code_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Error& error)
    {
        os << error.What();
        return os;
    }

private:
    std::error_code code_;
    std::string     info_;
};

} // namespace opengemini

namespace opengemini::errc {

enum class LogicErrors {
    NotImplemented = 1,
    InvalidArgument,
};

enum class ServerErrors {
    NoAvailableServer = 1,
    UnexpectedStatusCode,
    ErrorResult,
};

enum class RuntimeErrors {
    Unexpected = 1,
};

} // namespace opengemini::errc

#include "opengemini/impl/ErrorCode.hpp"

#endif // !OPENGEMINI_ERROR_HPP
