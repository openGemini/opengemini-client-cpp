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

#ifndef OPENGEMINI_IMPL_COMM_TASKSLOT_HPP
#define OPENGEMINI_IMPL_COMM_TASKSLOT_HPP

#include <boost/asio.hpp>

namespace opengemini::impl {

class TaskSlot {
public:
    TaskSlot(boost::asio::io_context& ctx) : ctx_(ctx) { }
    ~TaskSlot() = default;

private:
    TaskSlot(const TaskSlot&)                = delete;
    TaskSlot(TaskSlot&&) noexcept            = delete;
    TaskSlot& operator=(const TaskSlot&)     = delete;
    TaskSlot& operator=(TaskSlot&&) noexcept = delete;

protected:
    boost::asio::io_context& ctx_;
};

} // namespace opengemini::impl

#endif // !OPENGEMINI_IMPL_COMM_TASKSLOT_HPP
