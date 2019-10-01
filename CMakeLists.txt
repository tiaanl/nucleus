if(TARGET nucleus)
    return()
endif()

cmake_minimum_required(VERSION 3.7.2)
project(nucleus)

# Options

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
option(NUCLEUS_BUILD_TESTS "Build nucleus tests" ON)

# Third party libraries.

add_subdirectory("external")

# nucleus

set(nucleus_HEADER_FILES
    include/nucleus/Allocators/Allocator.h
    include/nucleus/Allocators/DebugAllocator.h
    include/nucleus/Allocators/DefaultAllocator.h
    include/nucleus/Allocators/GlobalAllocator.h
    include/nucleus/Allocators/LinearAllocator.h
    include/nucleus/Allocators/TestAllocator.h
    include/nucleus/ByteOrder.h
    include/nucleus/Callbacks/Bind.h
    include/nucleus/Callbacks/BindDetail.h
    include/nucleus/Callbacks/BindState.h
    include/nucleus/Callbacks/Callback.h
    include/nucleus/Callbacks/CallbackDetail.h
    include/nucleus/Callbacks/CallbackForward.h
    include/nucleus/Callbacks/ExtractReturnType.h
    include/nucleus/Callbacks/FunctorTraits.h
    include/nucleus/Callbacks/MakeBindStateType.h
    include/nucleus/Callbacks/MakeFunctionType.h
    include/nucleus/Callbacks/PassingTraits.h
    include/nucleus/Callbacks/TypeList.h
    include/nucleus/Config.h
    include/nucleus/Containers/Array.h
    include/nucleus/Containers/BitSet.h
    include/nucleus/Containers/DynamicArray.h
    include/nucleus/Containers/GrowingArray.h
    include/nucleus/Debugger.h
    include/nucleus/FilePath.h
    include/nucleus/HighPerformanceTimer.h
    include/nucleus/Logging.h
    include/nucleus/Macros.h
    include/nucleus/Memory/RefCountedPtr.h
    include/nucleus/Memory/ScopedPtr.h
    include/nucleus/Memory/ScopedRefPtr.h
    include/nucleus/MemoryDebug.h
    include/nucleus/Parser/Tokenizer.h
    include/nucleus/RefCounted.h
    include/nucleus/SourceLocation.h
    include/nucleus/Streams/ConsoleOutputStream.h
    include/nucleus/Streams/FileInputStream.h
    include/nucleus/Streams/FileOutputStream.h
    include/nucleus/Streams/InputStream.h
    include/nucleus/Streams/MemoryInputStream.h
    include/nucleus/Streams/OutputStream.h
    include/nucleus/Streams/StringOutputStream.h
    include/nucleus/Streams/Utils.h
    include/nucleus/Streams/WrappedDynamicArrayOutputStream.h
    include/nucleus/Streams/WrappedMemoryInputStream.h
    include/nucleus/Synchronization/AutoLock.h
    include/nucleus/Synchronization/Lock.h
    include/nucleus/Text/DynamicString.h
    include/nucleus/Text/StaticString.h
    include/nucleus/Text/String.h
    include/nucleus/Text/StringPool.h
    include/nucleus/Text/StringView.h
    include/nucleus/Types.h
    include/nucleus/Win/WindowsMixin.h
    # "include/nucleus/Parser/XMLParser.h"
    )

set(nucleus_SOURCE_FILES
    "src/Allocators/DebugAllocator.cpp"
    "src/Allocators/DefaultAllocator.cpp"
    "src/Allocators/GlobalAllocator.cpp"
    "src/Allocators/LinearAllocator.cpp"
    "src/Callbacks/CallbackDetail.cpp"
    "src/Debugger.cpp"
    "src/FilePath.cpp"
    "src/HighPerformanceTimer.cpp"
    "src/Logging.cpp"
    "src/Parser/Tokenizer.cpp"
    "src/Streams/ConsoleOutputStream.cpp"
    "src/Streams/FileInputStream.cpp"
    "src/Streams/FileOutputStream.cpp"
    "src/Streams/InputStream.cpp"
    "src/Streams/MemoryInputStream.cpp"
    "src/Streams/OutputStream.cpp"
    "src/Streams/StringOutputStream.cpp"
    "src/Streams/Utils.cpp"
    "src/Streams/WrappedMemoryInputStream.cpp"
    "src/Synchronization/Lock.cpp"
    "src/Text/DynamicString.cpp"
    "src/Text/StringView.cpp"
    # "src/Parser/XMLParser.cpp"
    )

set(nucleus_TESTS_SOURCE_FILES
    "tests/Allocators/AllocatorTests.cpp"
    "tests/Allocators/DebugAllocatorTests.cpp"
    "tests/Callbacks/CallbackTests.cpp"
    "tests/Containers/ArrayTests.cpp"
    "tests/Containers/BitSetTests.cpp"
    "tests/Containers/DynamicArrayTests.cpp"
    "tests/FilePathTests.cpp"
    "tests/HighPerformanceTimerTests.cpp"
    "tests/LoggingTests.cpp"
    "tests/LoggingTests.cpp"
    "tests/Memory/ScopedPtrTests.cpp"
    "tests/Memory/ScopedRefPtrTests.cpp"
    "tests/Parser/TokenizerTests.cpp"
    # "tests/Parser/XMLParserTests.cpp"
    "tests/RefCountedTests.cpp"
    "tests/Streams/ConsoleOutputStreamTests.cpp"
    "tests/Streams/StringOutputStreamTests.cpp"
    "tests/Text/DynamicStringTests.cpp"
    "tests/Text/StaticStringTests.cpp"
    "tests/Text/StringPoolTests.cpp"
    "tests/Text/StringTests.cpp"
    "tests/Text/StringViewTests.cpp"
    "tests/Containers/GrowingArrayTests.cpp"
    )

if(WIN32)
    list(APPEND nucleus_SOURCE_FILES
        "src/Streams/FileInputStream_win.cpp"
        )
endif()

if(UNIX)
    list(APPEND nucleus_SOURCE_FILES
        "src/Streams/FileInputStream_posix.cpp"
        )
endif()

add_library(nucleus STATIC ${nucleus_HEADER_FILES} ${nucleus_SOURCE_FILES})

target_include_directories(nucleus PUBLIC "${PROJECT_SOURCE_DIR}/include")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Enable warnings and multi-process compilation.
    target_compile_options(nucleus PUBLIC /W3 /WX /MP)

    # Enable memory debugging.
    target_compile_definitions(nucleus PUBLIC "-D_CRTDBG_MAP_ALLOC")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    # Enable warnings.
    target_compile_options(nucleus PUBLIC -Wall -Wextra -pedantic -Werror)
endif()

set_target_properties(nucleus PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER nucleus
    )

if(NUCLEUS_BUILD_TESTS)
    add_executable(nucleusTests ${nucleus_TESTS_SOURCE_FILES})
    target_link_libraries(nucleusTests nucleus testing)
    set_target_properties(nucleusTests PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        FOLDER nucleus
        )

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        # Enable warnings and multi-process compilation.
        target_compile_options(nucleusTests PUBLIC /W3 /WX /MP)
    
        # Enable memory debugging.
        target_compile_definitions(nucleusTests PUBLIC "-D_CRTDBG_MAP_ALLOC")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        # Enable warnings.
        target_compile_options(nucleusTests PUBLIC -Wall -Wextra -pedantic -Werror)
    endif()
endif()

if(WIN32)
    add_subdirectory(benchmarks "benchmarks")
endif()
