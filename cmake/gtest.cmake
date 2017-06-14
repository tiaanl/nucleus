
add_subdirectory("third_party/googltest-1.8.0" "gtest")
target_include_directories("gtest" PUBLIC "${gtest_SOURCE_DIR}/include")
set_property(TARGET "gtest" PROPERTY FOLDER "gtest")
set_property(TARGET "gtest_main" PROPERTY FOLDER "gtest")
set_property(TARGET "gmock" PROPERTY FOLDER "gtest")
set_property(TARGET "gmock_main" PROPERTY FOLDER "gtest")
