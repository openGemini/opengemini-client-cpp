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

#ifndef OPENGEMINI_IMPL_ENC_LINEPROTOCOLENCODER_HPP
#define OPENGEMINI_IMPL_ENC_LINEPROTOCOLENCODER_HPP

#include <sstream>
#include <string_view>
#include <vector>

#include "opengemini/Point.hpp"
#include "opengemini/impl/util/Preprocessor.hpp"

namespace opengemini::impl::enc {

class LineProtocolEncoder {
public:
    std::string Encode(const Point& point);
    std::string Encode(const std::vector<Point>& points);

private:
    void AppendPoint(const Point& point);

    void AppendMeasurement(std::string_view measurement);
    void AppendTags(const decltype(Point::tags)& tags);
    void AppendFields(const decltype(Point::fields)& fields);
    void AppendTimestamp(const Point::Time& time, Precision precision);

    void AppendField(const decltype(Point::fields)::value_type& field);
    void AppendEscapeString(std::string_view origin, std::string_view escapes);

    template<typename T>
    void Append(T&& t)
    {
        os_ << std::forward<T>(t);
    }

private:
    std::ostringstream os_;

    static constexpr auto ELEMENT_LF{ '\n' };
    static constexpr auto ELEMENT_EOF{ '\0' };
    static constexpr auto ELEMENT_COMMA{ ',' };
    static constexpr auto ELEMENT_EQUAL{ '=' };
    static constexpr auto ELEMENT_SPACE{ ' ' };
    static constexpr auto ELEMENT_DQUOTE{ '"' };
    static constexpr auto ELEMENT_BSLASH{ '\\' };

    static constexpr auto ELEMENT_UINT{ 'u' };
    static constexpr auto ELEMENT_INT{ 'i' };
    static constexpr auto ELEMENT_TRUE{ 'T' };
    static constexpr auto ELEMENT_FALSE{ 'F' };

    static constexpr char ESCAPE_CHARS_TAG[]{
        ELEMENT_COMMA,
        ELEMENT_EQUAL,
        ELEMENT_SPACE,
        ELEMENT_EOF,
    };

    static constexpr char ESCAPE_CHARS_FIELD_KEY[]{
        ELEMENT_COMMA,
        ELEMENT_EQUAL,
        ELEMENT_SPACE,
        ELEMENT_EOF,
    };
    static constexpr char ESCAPE_CHARS_FIELD_VALUE[]{
        ELEMENT_DQUOTE,
        ELEMENT_BSLASH,
        ELEMENT_EOF,
    };

    static constexpr char ESCAPE_CHARS_MEASUREMENT[]{
        ELEMENT_COMMA,
        ELEMENT_SPACE,
        ELEMENT_EOF,
    };
};

} // namespace opengemini::impl::enc

#ifndef OPENGEMINI_SEPARATE_COMPILATION
#    include "opengemini/impl/enc/LineProtocolEncoder.cpp"
#endif // !OPENGEMINI_SEPARATE_COMPILATION
//
#endif // !OPENGEMINI_IMPL_ENC_LINEPROTOCOLENCODER_HPP
