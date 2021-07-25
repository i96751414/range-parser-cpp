#include "gtest/gtest.h"

#include "range_parser/range_parser.hpp"

class InvalidRangeTest : public testing::TestWithParam<const char *> {
};

TEST_P(InvalidRangeTest, Should_Throw_When_InvalidRangeProvided) {
    EXPECT_THROW({ range_parser::parse(GetParam(), 10); }, range_parser::RangeException);
}

INSTANTIATE_TEST_SUITE_P(InvalidRanges, InvalidRangeTest, testing::Values(
        "", "bytes=0", "bytes=,", "bytes=--1", "bytes=-a", "bytes=a-", "bytes=10,9", "bytes=-1-2", "bytes=1--2",
        "bytes=1.1-2", "bytes=1-2.1", "bytes=(1)-2"));

class IgnoredRangeTest : public testing::TestWithParam<const char *> {
};

TEST_P(IgnoredRangeTest, Should_IgnoreRange_When_RangeIsEmpty_Or_StartGreaterOrEqualThanSize) {
    auto range = range_parser::parse(GetParam(), 10);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_TRUE(range.ranges.empty());
}

INSTANTIATE_TEST_SUITE_P(EmptyRanges, IgnoredRangeTest, testing::Values(
        "bytes=", "bytes=10-", "bytes=11-", "bytes=10-11", "bytes=11-12"));

typedef std::tuple<std::string, std::int64_t, std::int64_t, std::int64_t> SingularRangeParams;

class SingularRangeTest : public testing::TestWithParam<SingularRangeParams> {
};

TEST_P(SingularRangeTest, Should_ParseRange_When_ValidRangeProvided) {
    auto range = range_parser::parse(std::get<0>(GetParam()), std::get<1>(GetParam()));
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 1);
    EXPECT_EQ(range.ranges.at(0).start, std::get<2>(GetParam()));
    EXPECT_EQ(range.ranges.at(0).length, std::get<3>(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(ValidRanges, SingularRangeTest, testing::Values(
        // range, size, r.start, r.length
        // Suffix length ranges
        SingularRangeParams("bytes=-40", 100, 60, 40),
        SingularRangeParams("bytes=-200", 100, 0, 100),
        // End omitted ranges
        SingularRangeParams("bytes=0-", 100, 0, 100),
        SingularRangeParams("bytes=40-", 100, 40, 60),
        SingularRangeParams("bytes=99-", 100, 99, 1),
        // Simple ranges
        SingularRangeParams("bytes=0-59", 200, 0, 60),
        SingularRangeParams("bytes=60-119", 200, 60, 60),
        SingularRangeParams("bytes=120-199", 200, 120, 80),
        SingularRangeParams("bytes=120-200", 200, 120, 80),
        SingularRangeParams("bytes=120-210", 200, 120, 80)));

TEST(MultipleRangeTest, Should_ParseRanges_When_TwoValidRangesProvided) {
    auto range = range_parser::parse("bytes=0-49,60-", 100);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 2);
    EXPECT_EQ(range.ranges.at(0).start, 0);
    EXPECT_EQ(range.ranges.at(0).length, 50);
    EXPECT_EQ(range.ranges.at(1).start, 60);
    EXPECT_EQ(range.ranges.at(1).length, 40);
}