
project("googletest")

set(googletest_SOURCE_FILES
    "googletest/src/gtest-all.cc"
    "googlemock/src/gmock-all.cc"
    "googlemock/src/gmock_main.cc"
    )

add_library(googletest ${googletest_SOURCE_FILES})
target_include_directories(googletest PUBLIC "${googletest_SOURCE_DIR}/googlemock/include")
target_include_directories(googletest PRIVATE "${googletest_SOURCE_DIR}/googlemock")
target_include_directories(googletest PUBLIC "${googletest_SOURCE_DIR}/googletest/include")
target_include_directories(googletest PRIVATE "${googletest_SOURCE_DIR}/googletest")
set_property(TARGET googletest PROPERTY FOLDER third_party)
