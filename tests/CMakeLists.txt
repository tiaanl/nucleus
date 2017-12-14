
# nucleus_tests

set(nucleus_TEST_FILES
    "Allocators/AllocatorTests.cpp"
    "Containers/DynamicArrayTests.cpp"
    "Files/FilePathTests.cpp"
    "LoggingTests.cpp"
    "Memory/RefCountedTests.cpp"
    "Memory/ScopedPtrTests.cpp"
    "OptionalTests.cpp"
    "Utils/SignalsTests.cpp"
    )

add_executable(nucleus_tests ${nucleus_TEST_FILES})
target_link_libraries(nucleus_tests nucleus googletest)
set_property(TARGET nucleus_tests PROPERTY FOLDER nucleus)
