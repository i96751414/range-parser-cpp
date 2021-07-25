#ifndef RANGE_PARSER_HPP
#define RANGE_PARSER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>

namespace range_parser {

    class RangeException : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct Range {
        std::int64_t start;
        std::int64_t length;

        Range(std::int64_t pStart, std::int64_t pLength)
                : start(pStart), length(pLength) {}
    };

    struct HTTPRange {
        std::string unit;
        std::vector<Range> ranges;
    };

    HTTPRange parse(const std::string &pRange, std::int64_t pSize);

}

#endif //RANGE_PARSER_HPP
