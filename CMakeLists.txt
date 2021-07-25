cmake_minimum_required(VERSION 3.16)
project(range_parser
        DESCRIPTION "C++ small library for parsing HTTP range headers"
        HOMEPAGE_URL "https://github.com/i96751414/range-parser-cpp")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(build_tests "build tests" OFF)

if (build_tests)
    enable_testing()
    add_subdirectory(test)
endif ()

set(sources src/range_parser.cpp)
set(headers include/range_parser/range_parser.hpp)

add_library(range_parser ${sources} ${headers})

target_include_directories(range_parser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS range_parser EXPORT RangeParserTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/range_parser
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h*")
