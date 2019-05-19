
# nucleusTests

set(nucleus_TESTS_INCLUDE_FILES
)

set(nucleus_TESTS_SOURCE_FILES
    #"Allocators/AllocatedTests.cpp"
    #"Allocators/AllocatorTests.cpp"
    #"Allocators/DebugAllocatorTests.cpp"
    #"AtExitTests.cpp"
    #"Callbacks/CallbackTests.cpp"
    #"Containers/ArrayTests.cpp"
    #"Containers/BitSetTests.cpp"
    #"Containers/DynamicArrayTests.cpp"
    #"FilePathTests.cpp"
    #"HighPerformanceTimerTests.cpp"
    #"LoggingTests.cpp"
    #"LoggingTests.cpp"
    #"Memory/ScopedPtrTests.cpp"
    #"MessageLoop/MessageLoopTests.cpp"
    #"OptionalTests.cpp"
    #"Parser/ParserTests.cpp"
    #"RefCountedTests.cpp"
    #"SignalsTests.cpp"
    #"SingletonTests.cpp"
    #"Text/StringTests.cpp"
    #"FunctionTests.cpp"
    "Text/StringViewTests.cpp"
)

add_executable(nucleusTests ${nucleus_TESTS_INCLUDE_FILES} ${nucleus_TESTS_SOURCE_FILES})
target_link_libraries(nucleusTests nucleus testing)
set_target_properties(nucleusTests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER nucleus
    )
if(MSVC)
    target_compile_options(nucleusTests PUBLIC "/MP")
    target_compile_definitions(nucleusTests PUBLIC "-D_CRTDBG_MAP_ALLOC")
endif()
