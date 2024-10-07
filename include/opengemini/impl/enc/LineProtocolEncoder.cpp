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

#include "opengemini/impl/enc/LineProtocolEncoder.hpp"

#include <algorithm>
#include <cassert>
#include <type_traits>

#include "opengemini/Exception.hpp"
#include "opengemini/Precision.hpp"

namespace opengemini::impl::enc {

namespace {

template<typename DURATION>
inline auto Round(const Point::Time& time)
{
    return std::chrono::time_point_cast<Point::Time::duration>(
               std::chrono::round<DURATION>(time))
        .time_since_epoch()
        .count();
}

} // namespace

OPENGEMINI_INLINE_SPECIFIER
std::string LineProtocolEncoder::Encode(const Point& point)
{
    AppendPoint(point);
    return os_.str();
}

OPENGEMINI_INLINE_SPECIFIER
std::string LineProtocolEncoder::Encode(const std::vector<Point>& points)
{
    for (auto& point : points) {
        AppendPoint(point);
        Append(ELEMENT_LF);
    }

    return os_.str();
}

OPENGEMINI_INLINE_SPECIFIER
void LineProtocolEncoder::AppendPoint(const Point& point)
{
    auto& [measurement, fields, time, tags, precision] = point;
    AppendMeasurement(measurement);
    AppendTags(tags);
    AppendFields(fields);
    AppendTimestamp(time, precision);
}

OPENGEMINI_INLINE_SPECIFIER
void LineProtocolEncoder::AppendMeasurement(std::string_view measurement)
{
    if (measurement.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "The filed <measurement> in Point must not be empty");
    }

    AppendEscapeString(measurement, ESCAPE_CHARS_MEASUREMENT);
}

OPENGEMINI_INLINE_SPECIFIER
void LineProtocolEncoder::AppendTags(const decltype(Point::tags)& tags)
{
    for (auto& [key, value] : tags) {
        Append(ELEMENT_COMMA);
        AppendEscapeString(key, ESCAPE_CHARS_TAG);
        Append(ELEMENT_EQUAL);
        AppendEscapeString(value, ESCAPE_CHARS_TAG);
    }
}

OPENGEMINI_INLINE_SPECIFIER
void LineProtocolEncoder::AppendFields(const decltype(Point::fields)& fields)
{
    if (fields.empty()) {
        throw Exception(errc::LogicErrors::InvalidArgument,
                        "The filed <fields> in Point must not be empty");
    }

    Append(ELEMENT_SPACE);
    std::for_each_n(fields.begin(),
                    fields.size() - 1,
                    [this](const auto& field) {
                        AppendField(field);
                        Append(ELEMENT_COMMA);
                    });
    AppendField(*fields.rbegin());
}

OPENGEMINI_INLINE_SPECIFIER
void LineProtocolEncoder::AppendTimestamp(const Point::Time& time,
                                          Precision          precision)
{
    using namespace std::chrono;

    int64_t count{ 0 };
    switch (precision) {
    case Precision::Nanosecond: count = time.time_since_epoch().count(); break;
    case Precision::Microsecond: count = Round<microseconds>(time); break;
    case Precision::Millisecond: count = Round<milliseconds>(time); break;
    case Precision::Second: count = Round<seconds>(time); break;
    case Precision::Minute: count = Round<minutes>(time); break;
    case Precision::Hour: count = Round<hours>(time); break;
    }

    if (count != 0) {
        Append(ELEMENT_SPACE);
        Append(count);
    }
}

OPENGEMINI_INLINE_SPECIFIER
void LineProtocolEncoder::AppendField(
    const decltype(Point::fields)::value_type& field)
{
    auto& [key, value] = field;

    AppendEscapeString(key, ESCAPE_CHARS_FIELD_KEY);
    Append(ELEMENT_EQUAL);
    std::visit(
        [this](const auto& innerValue) {
            using T = std::decay_t<decltype(innerValue)>;

            if constexpr (std::is_same_v<T, std::string>) {
                Append(ELEMENT_DQUOTE);
                AppendEscapeString(innerValue, ESCAPE_CHARS_FIELD_VALUE);
                Append(ELEMENT_DQUOTE);
            }
            else if constexpr (std::is_same_v<T, uint64_t>) {
                Append(innerValue);
                Append(ELEMENT_UINT);
            }
            else if constexpr (std::is_same_v<T, int64_t>) {
                Append(innerValue);
                Append(ELEMENT_INT);
            }
            else if constexpr (std::is_same_v<T, bool>) {
                Append(innerValue ? ELEMENT_TRUE : ELEMENT_FALSE);
            }
            else if constexpr (std::is_same_v<T, double>) {
                Append(innerValue);
            }
            else {
                throw Exception(errc::LogicErrors::NotImplemented,
                                "Only support std::string, uint64_t, int64_t, "
                                "bool, double as filed value type");
            }
        },
        value);
}

OPENGEMINI_INLINE_SPECIFIER
void LineProtocolEncoder::AppendEscapeString(std::string_view origin,
                                             std::string_view escapes)
{
    for (auto ch : origin) {
        if (escapes.find(ch) != escapes.npos) { Append(ELEMENT_BSLASH); }
        Append(ch);
    }
}

} // namespace opengemini::impl::enc
