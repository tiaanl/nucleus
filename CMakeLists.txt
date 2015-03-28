cmake_minimum_required(VERSION 3.0)
project("nucleus")

# Third party libraries.

add_subdirectory("third_party/gtest-1.7.0" "gtest")
include_directories("${gtest_SOURCE_DIR}/include")

# Add our local libraries.

include_directories("src")

add_subdirectory("src/base" "base")
