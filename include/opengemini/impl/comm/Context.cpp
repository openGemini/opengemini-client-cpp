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

#include "opengemini/impl/comm/Context.hpp"

#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl {

OPENGEMINI_INLINE_SPECIFIER
Context::Context(std::size_t concurrencyHint) :
    ctx_(ChooseThreadsNum(concurrencyHint)),
    ctxGuard_(boost::asio::make_work_guard(ctx_)),
    threads_(ConstructWorkingThreads(ChooseThreadsNum(concurrencyHint)))
{ }

OPENGEMINI_INLINE_SPECIFIER
Context::~Context()
{
    if (!ctx_.stopped()) { Shutdown(); }
}

OPENGEMINI_INLINE_SPECIFIER
void Context::Shutdown()
{
    ctxGuard_.reset();
    ctx_.stop();
    for (auto& thread : threads_) {
        if (thread.joinable()) { thread.join(); }
    }
}

OPENGEMINI_INLINE_SPECIFIER
std::vector<std::thread> Context::ConstructWorkingThreads(std::size_t threadNum)
{
    assert(threadNum > 0);
    std::vector<std::thread> threads;
    threads.reserve(threadNum);
    std::generate_n(std::back_inserter(threads), threadNum, [this] {
        return std::thread([this] {
            for (;;) {
                try {
                    ctx_.run();
                    break;
                }
                catch (...) {
                }
            }
        });
    });
    return threads;
}

OPENGEMINI_INLINE_SPECIFIER
std::size_t Context::ChooseThreadsNum(std::size_t concurrencyHint) noexcept
{
    constexpr auto min          = 1;
    const auto     hardwareHint = std::thread::hardware_concurrency();
    const auto     max          = hardwareHint * 4;
    if (concurrencyHint >= min && concurrencyHint <= max) {
        return concurrencyHint;
    }

    return hardwareHint == 0 ? min : hardwareHint;
}

OPENGEMINI_INLINE_SPECIFIER
boost::asio::io_context& Context::operator()() noexcept
{
    return ctx_;
}

} // namespace opengemini::impl
