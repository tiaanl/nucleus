
set(nucleus_TEST_FILES
    # "Atomics/AtomicsTests.cpp"

    "OptionalTests.cpp"

    "Allocators/AllocatorTests.cpp"

    "Memory/RefCountedTests.cpp"
    "Memory/ScopedPtrTests.cpp"

    "containers/array_tests.cpp"
    "containers/vector_tests.cpp"
    "files/file_path_tests.cpp"
    "logging_test.cpp"
    "strings/char_ptr_tests.cpp"
    "strings/string_utf_conversion_tests.cpp"
    "strings/string_utils_tests.cpp"
    "utils/observer_list_tests.cpp"
    "utils/signals_tests.cpp"
)

add_executable(nucleus_tests ${nucleus_TEST_FILES})
target_link_libraries("nucleus_tests" "nucleus" "googletest")
set_property(TARGET nucleus_tests PROPERTY FOLDER nucleus)
