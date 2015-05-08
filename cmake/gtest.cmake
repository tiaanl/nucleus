
add_subdirectory("third_party/gtest-1.7.0" "gtest")
target_include_directories("gtest" PUBLIC "${gtest_SOURCE_DIR}/include")
set_property(TARGET "gtest" PROPERTY FOLDER "gtest")
set_property(TARGET "gtest_main" PROPERTY FOLDER "gtest")
