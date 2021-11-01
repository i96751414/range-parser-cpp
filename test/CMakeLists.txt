cmake_minimum_required(VERSION 3.16)

set(this range_parser_test)

# Get GTest
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.11.0.tar.gz)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# find_package(GTest REQUIRED)

add_executable(${this} range_parser_test.cpp)

target_link_libraries(${this} PUBLIC
        gtest
        pthread
        gtest_main
        range_parser)

add_test(
        NAME ${this}
        COMMAND ${this})