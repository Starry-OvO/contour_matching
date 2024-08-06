cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(CM LANGUAGES CXX)

include(cmake/options.cmake)
include(cmake/dependencies.cmake)
include(cmake/helper.cmake)

add_subdirectory(lib)
if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_subdirectory(bin)
endif ()
