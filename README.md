# range-parser-cpp

[![Build Status](https://github.com/i96751414/range-parser-cpp/workflows/build/badge.svg)](https://github.com/i96751414/range-parser-cpp/actions?query=workflow%3Abuild)

A small C++ library for parsing HTTP range headers as specified in [RFC 7233](https://www.rfc-editor.org/info/rfc7233).

## Example

```c++
#include "range_parser/range_parser.hpp"

auto range = range_parser::parse("bytes=0-29", 100);

std::cout << "unit: " << range.unit << std::endl;
std::cout << "start: " << range.ranges.at(0).start << std::endl;
std::cout << "length: " << range.ranges.at(0).length << std::endl;
std::cout << "total length: " << range.total_length() << std::endl;
```