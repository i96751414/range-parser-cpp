cmake_minimum_required(VERSION 3.16)
project(range_parser
        VERSION 1.0.1
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

add_library(${PROJECT_NAME} ${sources} ${headers})

string(REGEX MATCH "[0-9]+\\.[0-9]+" SOVERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${SOVERSION})

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/range_parser
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h*")

# ---------------------------------------------------------------------------------------
# Install CMake config files
# ---------------------------------------------------------------------------------------

set(config_package_location "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake")
set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/config.cmake.in")

install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${config_package_location})

include(CMakePackageConfigHelpers)
configure_package_config_file("${project_config_in}" "${project_config_out}"
        INSTALL_DESTINATION "${config_package_location}")

write_basic_package_version_file("${version_config_file}" COMPATIBILITY AnyNewerVersion)

install(FILES "${project_config_out}" "${version_config_file}"
        DESTINATION ${config_package_location})
