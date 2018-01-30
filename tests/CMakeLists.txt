
# nucleusTests

set(nucleus_TESTS_INCLUDE_FILES
)

set(nucleus_TESTS_SOURCE_FILES
    # "Allocators/AllocatedTests.cpp"
    # "Allocators/AllocatorTests.cpp"
    # "Allocators/DebugAllocatorTests.cpp"
    # "Containers/ArrayTests.cpp"
    # "Containers/BitSetTests.cpp"
    # "Containers/DynamicArrayTests.cpp"
    # "FilePathTests.cpp"
    # "LoggingTests.cpp"
    # "OptionalTests.cpp"
    # "RefCountedTests.cpp"
    # "SignalsTests.cpp"
    # "Text/StringTests.cpp"

    "LoggingTests.cpp"
)

add_executable(nucleusTests ${nucleus_TESTS_INCLUDE_FILES} ${nucleus_TESTS_SOURCE_FILES})
target_link_libraries(nucleusTests nucleus testing)
set_property(TARGET nucleusTests PROPERTY FOLDER nucleus)
if(MSVC)
    target_compile_options(nucleusTests PUBLIC "/MP")
    target_compile_definitions(nucleusTests PUBLIC "-D_CRTDBG_MAP_ALLOC")
endif()
