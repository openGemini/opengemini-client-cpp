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

#include <gtest/gtest.h>

#include "opengemini/impl/enc/LineProtocolEncoder.hpp"
#include "test/ExpectThrowAs.hpp"

using namespace std::chrono_literals;

namespace opengemini::test {

using namespace opengemini::impl;

TEST(LineProtocolEncoderTest, WithoutEscapedChars)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } }),
        "test,T0=0 a=1i 1");
}

TEST(LineProtocolEncoderTest, WithEscapedCharsInMeasurement)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test,", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } }),
        R"(test\,,T0=0 a=1i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test ", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } }),
        R"(test\ ,T0=0 a=1i 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test, test",
                                                  { { "a", 1 } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test\,\ test,T0=0 a=1i 1)");
}

TEST(LineProtocolEncoderTest, WithEscapedCharsInTagKey)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0,", "0" } } }),
        R"(test,T0\,=0 a=1i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0=", "0" } } }),
        R"(test,T0\==0 a=1i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0 ", "0" } } }),
        R"(test,T0\ =0 a=1i 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", 1 } },
                                                  Point::Time{ 1ns },
                                                  { { "T,=T T", "0" } } }),
              R"(test,T\,\=T\ T=0 a=1i 1)");
}

TEST(LineProtocolEncoderTest, WithEscapedCharsInTagValue)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0," } } }),
        R"(test,T0=0\, a=1i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0=" } } }),
        R"(test,T0=0\= a=1i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0 " } } }),
        R"(test,T0=0\  a=1i 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", 1 } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0=0,0 0" } } }),
              R"(test,T0=0\=0\,0\ 0 a=1i 1)");
}

TEST(LineProtocolEncoderTest, WithEscapedCharsInFieldKey)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a,", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } }),
        R"(test,T0=0 a\,=1i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a=", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } }),
        R"(test,T0=0 a\==1i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a ", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } }),
        R"(test,T0=0 a\ =1i 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a,= a a ", 1 } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a\,\=\ a\ a\ =1i 1)");
}

TEST(LineProtocolEncoderTest, WithEscapedCharsInFieldValue)
{
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", R"(1")" } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a="1\"" 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", R"(1\)" } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a="1\\" 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", R"(1\\)" } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a="1\\\\" 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", R"(1\")" } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a="1\\\"" 1)");
}

TEST(LineProtocolEncoderTest, WithDifferentFieldTypes)
{
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", 1.0 } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a=1 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", 1.2 } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a=1.2 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", 12.345678901234 } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a=12.3457 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", 12345 } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a=12345i 1)");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode({ "test",
                                            { { "a", 12345678901234567890ul } },
                                            Point::Time{ 1ns },
                                            { { "T0", "0" } } }),
        R"(test,T0=0 a=12345678901234567890u 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", true } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a=T 1)");

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode({ "test",
                                                  { { "a", false } },
                                                  Point::Time{ 1ns },
                                                  { { "T0", "0" } } }),
              R"(test,T0=0 a=F 1)");
}

TEST(LineProtocolEncoderTest, WithDifferentTimeUnderDefaultPrecision)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } }),
        "test,T0=0 a=1i 1");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1us }, { { "T0", "0" } } }),
        "test,T0=0 a=1i 1000");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1ms }, { { "T0", "0" } } }),
        "test,T0=0 a=1i 1000000");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1s }, { { "T0", "0" } } }),
        "test,T0=0 a=1i 1000000000");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1min }, { { "T0", "0" } } }),
        "test,T0=0 a=1i 60000000000");

    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "test", { { "a", 1 } }, Point::Time{ 1h }, { { "T0", "0" } } }),
        "test,T0=0 a=1i 3600000000000");
}

TEST(LineProtocolEncoderTest, WithDifferentPrecision)
{
    Point point{ "test",
                 { { "a", 1 } },
                 Point::Time{ 1h + 2min + 3s + 4ms + 5us + 6ns },
                 { { "T0", "0" } } };

    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode(point),
              "test,T0=0 a=1i 3723004005006");

    point.precision = Precision::Hour;
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode(point),
              "test,T0=0 a=1i 3600000000000");

    point.precision = Precision::Minute;
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode(point),
              "test,T0=0 a=1i 3720000000000");

    point.precision = Precision::Second;
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode(point),
              "test,T0=0 a=1i 3723000000000");

    point.precision = Precision::Millisecond;
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode(point),
              "test,T0=0 a=1i 3723004000000");

    point.precision = Precision::Microsecond;
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode(point),
              "test,T0=0 a=1i 3723004005000");
}

TEST(LineProtocolEncoderTest, WithEmptyMeasurement)
{
    EXPECT_THROW_AS(
        enc::LineProtocolEncoder{}.Encode(Point{ {}, { { "field0", 0 } } }),
        errc::LogicErrors::InvalidArgument);
}

TEST(LineProtocolEncoderTest, WithEmptyFields)
{
    EXPECT_THROW_AS(
        enc::LineProtocolEncoder{}.Encode(Point{ "test_measurement", {} }),
        errc::LogicErrors::InvalidArgument);
}

TEST(LineProtocolEncoderTest, WithMultiFields)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode({ "ExampleMeasurement",
                                            { { "field1", "val1" },
                                              { "field2", 2222 },
                                              { "field3", 0.333 },
                                              { "field4", true },
                                              { "field5", -5555 },
                                              { "field6", 6666ul } },
                                            Point::Time{ 1ns } }),
        R"(ExampleMeasurement field1="val1",field2=2222i,field3=0.333,field4=T,field5=-5555i,field6=6666u 1)");
}

TEST(LineProtocolEncoderTest, WithoutTime)
{
    EXPECT_EQ(enc::LineProtocolEncoder{}.Encode(
                  { "ExampleMeasurement", { { "field1", "val1" } } }),
              R"(ExampleMeasurement field1="val1")");
}

TEST(LineProtocolEncoderTest, WithTags)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { "ExampleMeasurement",
              { { "field1", "val1" } },
              Point::Time{ 1ns },
              { { "tag1", "val1" }, { "tag2", "val2" }, { "tag3", "val3" } } }),
        R"(ExampleMeasurement,tag1=val1,tag2=val2,tag3=val3 field1="val1" 1)");
}

TEST(LineProtocolEncoderTest, WithMultiPoints)
{
    EXPECT_EQ(
        enc::LineProtocolEncoder{}.Encode(
            { { "test", { { "a", 1 } }, Point::Time{ 1ns }, { { "T0", "0" } } },
              { "test,",
                { { "a", 1 } },
                Point::Time{ 1ns },
                { { "T0", "0" } } },
              { "test",
                { { "a", 1 } },
                Point::Time{ 1ns },
                { { "T0,", "0" } } },
              { "test",
                { { "a", 1 } },
                Point::Time{ 1ns },
                { { "T0", "0," } } },
              { "test",
                { { "a,", 1 } },
                Point::Time{ 1ns },
                { { "T0", "0" } } },
              { "test",
                { { "a", R"(1")" } },
                Point::Time{ 1ns },
                { { "T0", "0" } } } }),
        "test,T0=0 a=1i 1\ntest\\,,T0=0 a=1i 1\ntest,T0\\,=0 a=1i "
        "1\ntest,T0=0\\, a=1i 1\ntest,T0=0 a\\,=1i 1\ntest,T0=0 a=\"1\\\"\" "
        "1\n");
}

TEST(LineProtocolEncoderTest, WithMultiPointsActuallyEmpty)
{
    EXPECT_TRUE(
        enc::LineProtocolEncoder{}.Encode(std::vector<Point>{}).empty());
}

} // namespace opengemini::test
