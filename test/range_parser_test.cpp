#include "gtest/gtest.h"

#include "range_parser/range_parser.hpp"

TEST(RangeTest, Should_Throw_When_NoUnitProvided) {
    EXPECT_THROW({ range_parser::parse("", 10); }, range_parser::RangeException);
}

TEST(RangeTest, Should_Throw_When_InvalidRangeProvided) {
    EXPECT_THROW({ range_parser::parse("bytes=0", 10); }, range_parser::RangeException);
    EXPECT_THROW({ range_parser::parse("bytes=,", 10); }, range_parser::RangeException);
    EXPECT_THROW({ range_parser::parse("bytes=--1", 10); }, range_parser::RangeException);
    EXPECT_THROW({ range_parser::parse("bytes=-a", 10); }, range_parser::RangeException);
    EXPECT_THROW({ range_parser::parse("bytes=10,9", 10); }, range_parser::RangeException);
}

TEST(RangeTest, Should_ParseRange_When_EmptyRangeProvided) {
    auto range = range_parser::parse("bytes=", 10);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_TRUE(range.ranges.empty());
}

TEST(RangeTest, Should_ParseRange_When_SuffixLengthProvided) {
    auto range = range_parser::parse("bytes=-40", 100);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 1);
    EXPECT_EQ(range.ranges.at(0).start, 60);
    EXPECT_EQ(range.ranges.at(0).length, 40);
}

TEST(RangeTest, Should_ParseRange_When_SuffixLengthGreaterThanSizeProvided) {
    auto range = range_parser::parse("bytes=-200", 100);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 1);
    EXPECT_EQ(range.ranges.at(0).start, 0);
    EXPECT_EQ(range.ranges.at(0).length, 100);
}

class IgnoredRangeTest : public testing::TestWithParam<const char *> {
};

TEST_P(IgnoredRangeTest, Should_IgnoreRange_When_StartGreaterOrEqualThanSize) {
    auto range = range_parser::parse(GetParam(), 10);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 0);
}

INSTANTIATE_TEST_SUITE_P(Ranges,
                         IgnoredRangeTest,
                         testing::Values("bytes=10-", "bytes=11-", "bytes=10-11", "bytes=11-12"));

class EndOmittedRangeTest : public testing::TestWithParam<std::int64_t> {
};

TEST_P(EndOmittedRangeTest, Should_ParseRange_When_EndOmitted) {
    auto range = range_parser::parse("bytes=" + std::to_string(GetParam()) + "-", 100);
    EXPECT_EQ(range.unit, "bytes");
    EXPECT_EQ(range.ranges.size(), 1);
    EXPECT_EQ(range.ranges.at(0).start, GetParam());
    EXPECT_EQ(range.ranges.at(0).length, 100 - GetParam());
}

INSTANTIATE_TEST_SUITE_P(Ranges, EndOmittedRangeTest, testing::Values(0, 40, 99));