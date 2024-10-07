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

#include "opengemini/Precision.hpp"

namespace opengemini {

constexpr auto ToString(Precision precision) noexcept
{
    switch (precision) {
    case Precision::Nanosecond: return "ns";
    case Precision::Microsecond: return "u";
    case Precision::Millisecond: return "ms";
    case Precision::Second: return "s";
    case Precision::Minute: return "m";
    case Precision::Hour: return "h";
    default: return "ns";
    }
}

} // namespace opengemini
