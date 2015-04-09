cmake_minimum_required(VERSION 3.0)
project("base")

set("base_SOURCE_FILES"
  "config.h"
  "files/file_path.cpp"
  "files/file_path.h"
  "files/file_utils.h"
  "logging.cpp"
  "logging.h"
  "macros.h"
  "message_loop/message_loop.cpp"
  "message_loop/message_loop.h"
  "strings/char_ptr.h"
  "strings/char_ptr_traits_16.h"
  "strings/char_ptr_traits_32.h"
  "strings/char_ptr_traits_8.h"
  "strings/char_ptr_traits_a.h"
  "strings/char_ptr_traits_w.h"
  "strings/string_utf_conversion.cpp"
  "strings/string_utf_conversion.h"
  "strings/string_utils.cpp"
  "strings/string_utils.h"
)

if(WIN32)
  list(APPEND "base_SOURCE_FILES"
    "files/file_utils_win.cpp"
  )
endif()

set("base_TEST_FILES"
  "logging_test.cpp"
  "message_loop/message_loop_tests.cpp"
  "strings/char_ptr_tests.cpp"
  "strings/string_utf_conversion_tests.cpp"
  "strings/string_utils_tests.cpp"
)

add_library("base" STATIC ${base_SOURCE_FILES})

add_executable("base_tests" ${base_TEST_FILES})
target_link_libraries("base_tests" "gtest" "gtest_main" "base")
