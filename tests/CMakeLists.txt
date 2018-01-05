
# nucleus_tests

set(nucleus_TEST_INCLUDE_FILES
)

set(nucleus_TEST_SOURCE_FILES
    "Allocators/AllocatedTests.cpp"
    "Allocators/AllocatorTests.cpp"
    "Allocators/DebugAllocatorTests.cpp"
    "Containers/ArrayTests.cpp"
    "Containers/BitSetTests.cpp"
    "Containers/DynamicArrayTests.cpp"
    "FilePathTests.cpp"
    "LoggingTests.cpp"
    "OptionalTests.cpp"
    "RefCountedTests.cpp"
    "SignalsTests.cpp"
    "Text/StringTests.cpp"
)

add_executable(nucleus_tests ${nucleus_TEST_INCLUDE_FILES} ${nucleus_TEST_SOURCE_FILES})
target_link_libraries(nucleus_tests nucleus googletest)
set_property(TARGET nucleus_tests PROPERTY FOLDER nucleus)
