
project("googletest")

set(googletest_SOURCE_FILES
    "src/gtest-all.cc"
    "src/gmock-all.cc"
    "src/gmock_main.cc"
)

add_library(googletest ${googletest_SOURCE_FILES})
target_include_directories(googletest PUBLIC "${googletest_SOURCE_DIR}/include")
target_include_directories(googletest PRIVATE "${googletest_SOURCE_DIR}")
set_property(TARGET googletest PROPERTY FOLDER third_party)
