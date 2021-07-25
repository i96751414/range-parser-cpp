cmake_minimum_required(VERSION 3.16)

set(this range_parser_test)

find_package(GTest REQUIRED)

add_executable(${this} range_parser_test.cpp)

target_link_libraries(${this} PUBLIC
        gtest
        pthread
        gtest_main
        range_parser)

add_test(
        NAME ${this}
        COMMAND ${this})