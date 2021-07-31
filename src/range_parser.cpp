#include "range_parser/range_parser.hpp"

#include <sstream>

namespace range_parser {

    inline void trim(std::string &str, const char *pChars = " ") {
        str.erase(0, str.find_first_not_of(pChars));
        str.erase(str.find_last_not_of(pChars) + 1);
    }

    inline std::int64_t parse_int(const std::string &pValue) {
        std::int64_t value;
        std::istringstream iss(pValue);
        iss >> value;

        if (iss.fail() || !iss.eof()) {
            throw RangeException("Invalid range: unable to parse range limit");
        }

        return value;
    }

    HTTPRange parse(const std::string &pRange, std::int64_t pSize) {
        auto unitIndex = pRange.find('=');
        if (unitIndex == std::string::npos) {
            throw RangeException("Invalid range: unable to find unit");
        }

        HTTPRange httpRange{pRange.substr(0, unitIndex), std::vector<Range>()};
        std::stringstream ss(pRange.substr(unitIndex + 1));
        std::string segment;

        while (std::getline(ss, segment, ',')) {
            auto index = segment.find('-');
            if (index == std::string::npos) {
                throw RangeException("Invalid range: unable to find limits");
            }

            auto start = segment.substr(0, index);
            auto end = segment.substr(index + 1);
            trim(start);
            trim(end);

            if (start.empty()) {
                if (end.empty() || end.at(0) == '-') {
                    throw RangeException("Invalid suffix length");
                }
                auto endOffset = std::min(pSize, parse_int(end));
                httpRange.ranges.emplace_back(pSize - endOffset, endOffset);
            } else {
                auto startOffset = parse_int(start);
                if (startOffset >= pSize) {
                    continue;
                }

                if (end.empty()) {
                    httpRange.ranges.emplace_back(startOffset, pSize - startOffset);
                } else {
                    auto endOffset = parse_int(end);
                    if (startOffset > endOffset) {
                        throw RangeException("Invalid range interval");
                    }
                    httpRange.ranges.emplace_back(startOffset, std::min(pSize, endOffset + 1) - startOffset);
                }
            }
        }

        return std::move(httpRange);
    }

    std::int64_t HTTPRange::total_length() {
        std::int64_t length(0);
        for (auto &r : ranges) {
            length += r.length;
        }
        return length;
    }

}
