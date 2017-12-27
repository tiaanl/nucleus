
# nucleus_tests

set(nucleus_TEST_INCLUDE_FILES
)

set(nucleus_TEST_SOURCE_FILES
    "Allocators/AllocatorTests.cpp"
    "Containers/ArrayTests.cpp"
    "Containers/BitSetTests.cpp"
    "Containers/DynamicArrayTests.cpp"
    "Files/FilePathTests.cpp"
    "LoggingTests.cpp"
    "Memory/AllocatedTests.cpp"
    "RefCountedTests.cpp"
    "Memory/ScopedPtrTests.cpp"
    "OptionalTests.cpp"
    "Text/StringTests.cpp"
    "Utils/SignalsTests.cpp"
)

add_executable(nucleus_tests ${nucleus_TEST_INCLUDE_FILES} ${nucleus_TEST_SOURCE_FILES})
target_link_libraries(nucleus_tests nucleus googletest)
set_property(TARGET nucleus_tests PROPERTY FOLDER nucleus)
