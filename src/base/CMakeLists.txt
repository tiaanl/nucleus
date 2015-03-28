cmake_minimum_required(VERSION 3.0)
project("base")

set("base_SOURCE_FILES"
  "config.h"
  "logging.cpp"
  "logging.h"
  "macros.h"
  "message_loop/message_loop.cpp"
  "message_loop/message_loop.h"
)

set("base_TEST_FILES"
  "logging_test.cpp"
  "message_loop/message_loop_tests.cpp"
)

add_library("base" ${base_SOURCE_FILES})

add_executable("base_tests" ${base_TEST_FILES})
target_link_libraries("base_tests" "gtest" "gtest_main" "base")
