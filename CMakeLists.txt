cmake_minimum_required(VERSION 3.0)
project("nucleus")

# Compiler options
if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()

# Third party libraries.

add_subdirectory("third_party/gtest-1.7.0" "gtest")
include_directories("${gtest_SOURCE_DIR}/include")

# Add our local libraries.

include_directories("src")

add_subdirectory("src/base" "base")
