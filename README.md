# range-parser-cpp

[![Build Status](https://github.com/i96751414/range-parser-cpp/workflows/build/badge.svg)](https://github.com/i96751414/range-parser-cpp/actions?query=workflow%3Abuild)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/76f16b060d254103aedb27b077c6ee13)](https://www.codacy.com/gh/i96751414/range-parser-cpp/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=i96751414/range-parser-cpp&amp;utm_campaign=Badge_Grade)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/i96751414/range-parser-cpp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/i96751414/range-parser-cpp/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/i96751414/range-parser-cpp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/i96751414/range-parser-cpp/context:cpp)

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