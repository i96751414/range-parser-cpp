#include "gtest/gtest.h"

#include "range_parser/range_parser.hpp"

class InvalidRangeTest : public testing::TestWithParam<const char *> {
};

TEST_P(InvalidRangeTest, Should_Throw_When_InvalidRangeProvided) {
    EXPECT_THROW({ range_parser::parse(GetParam(), 10); }, range_parser::RangeException);
}

INSTANTIATE_TEST_SUITE_P(InvalidRanges, InvalidRangeTest, testing::Values(
        "", "bytes=0", "bytes=,", "bytes=--1", "bytes=-a", "bytes=a-", "bytes=10,9", "bytes=-1-2", "bytes=1--2",
        "bytes=1.1-2", "bytes=1-2.1", "bytes=(1)-2", "bytes=2-1"));

class IgnoredRangeTest : public testing::TestWithParam<const char *> {
};

TEST_P(IgnoredRangeTest, Should_IgnoreRange_When_RangeIsEmpty_Or_StartGreaterOrEqualThanSize) {
    auto range = range_parser::parse(GetParam(), 10);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_TRUE(range.ranges.empty());
    EXPECT_EQ(range.total_length(), 0);
}

INSTANTIATE_TEST_SUITE_P(EmptyRanges, IgnoredRangeTest, testing::Values(
        "bytes=", "bytes=10-", "bytes=11-", "bytes=10-11", "bytes=11-12"));

typedef std::tuple<std::string, std::int64_t, std::int64_t, std::int64_t, std::string> SingularRangeParams;

class SingularRangeTest : public testing::TestWithParam<SingularRangeParams> {
};

TEST_P(SingularRangeTest, Should_ParseRange_When_ValidRangeProvided) {
    auto size = std::get<1>(GetParam());
    auto expectedLength = std::get<3>(GetParam());
    auto range = range_parser::parse(std::get<0>(GetParam()), size);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 1);
    EXPECT_EQ(range.ranges.at(0).start, std::get<2>(GetParam()));
    EXPECT_EQ(range.ranges.at(0).length, expectedLength);
    EXPECT_EQ(range.ranges.at(0).content_range(size), std::get<4>(GetParam()));
    EXPECT_EQ(range.total_length(), expectedLength);
}

INSTANTIATE_TEST_SUITE_P(ValidRanges, SingularRangeTest, testing::Values(
        // range, size, r.start, r.length, r.content_range
        // Suffix length ranges
        SingularRangeParams("bytes=-40", 100, 60, 40, "bytes 60-99/100"),
        SingularRangeParams("bytes=-200", 100, 0, 100, "bytes 0-99/100"),
        // End omitted ranges
        SingularRangeParams("bytes=0-", 100, 0, 100, "bytes 0-99/100"),
        SingularRangeParams("bytes=40-", 100, 40, 60, "bytes 40-99/100"),
        SingularRangeParams("bytes=99-", 100, 99, 1, "bytes 99-99/100"),
        // Simple ranges
        SingularRangeParams("bytes=0-0", 200, 0, 1, "bytes 0-0/200"),
        SingularRangeParams("bytes=0-59", 200, 0, 60, "bytes 0-59/200"),
        SingularRangeParams("bytes=60-119", 200, 60, 60, "bytes 60-119/200"),
        SingularRangeParams("bytes=120-199", 200, 120, 80, "bytes 120-199/200"),
        SingularRangeParams("bytes=120-200", 200, 120, 80, "bytes 120-199/200"),
        SingularRangeParams("bytes=120-210", 200, 120, 80, "bytes 120-199/200")));

TEST(MultipleRangeTest, Should_ParseRanges_When_TwoValidRangesProvided) {
    auto range = range_parser::parse("bytes=0-49,60-", 100);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 2);
    EXPECT_EQ(range.ranges.at(0).start, 0);
    EXPECT_EQ(range.ranges.at(0).length, 50);
    EXPECT_EQ(range.ranges.at(1).start, 60);
    EXPECT_EQ(range.ranges.at(1).length, 40);
    EXPECT_EQ(range.total_length(), 90);
}