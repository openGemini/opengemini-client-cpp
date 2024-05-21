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

#ifndef OPENGEMINI_IMPL_COMM_CONTEXT_HPP
#define OPENGEMINI_IMPL_COMM_CONTEXT_HPP

#include <thread>
#include <vector>

#include <boost/asio.hpp>

namespace opengemini::impl {

class Context {
public:
    explicit Context(std::size_t concurrencyHint = 0);
    ~Context();

    boost::asio::io_context& operator()() noexcept;

private:
    Context(const Context&)                = delete;
    Context(Context&&) noexcept            = delete;
    Context& operator=(const Context&)     = delete;
    Context& operator=(Context&&) noexcept = delete;

    std::vector<std::thread> ConstructWorkingThreads(std::size_t threadNum);

    static std::size_t ChooseThreadsNum(std::size_t concurrencyHint) noexcept;

private:
    boost::asio::io_context ctx_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
                             ctxGuard_;
    std::vector<std::thread> threads_;
};

} // namespace opengemini::impl

#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/comm/Context.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION

#endif // !OPENGEMINI_IMPL_COMM_CONTEXT_HPP
