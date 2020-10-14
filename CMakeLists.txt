cmake_minimum_required(VERSION 3.16)

if (TARGET nucleus)
    return()
endif ()

project(nucleus)

include(cmake/Utils.cmake)

set(nucleus_HEADER_FILES
    include/nucleus/ByteOrder.h
    include/nucleus/Config.h
    include/nucleus/Containers/ArrayView.h
    include/nucleus/Containers/BitSet.h
    include/nucleus/Containers/DynamicArray.h
    include/nucleus/Containers/StaticArray.h
    include/nucleus/Debugger.h
    include/nucleus/FilePath.h
    include/nucleus/Filter.h
    include/nucleus/HighPerformanceTimer.h
    include/nucleus/Logging.h
    include/nucleus/Macros.h
    include/nucleus/Memory/RefCountedPtr.h
    include/nucleus/Memory/ScopedPtr.h
    include/nucleus/Memory/ScopedRefPtr.h
    include/nucleus/Parser/Tokenizer.h
    include/nucleus/Profiling.h
    include/nucleus/RefCounted.h
    include/nucleus/SourceLocation.h
    include/nucleus/Streams/ConsoleOutputStream.h
    include/nucleus/Streams/ArrayInputStream.h
    include/nucleus/Streams/DynamicBufferOutputStream.h
    include/nucleus/Streams/FileInputStream.h
    include/nucleus/Streams/FileOutputStream.h
    include/nucleus/Streams/InputStream.h
    include/nucleus/Streams/MemoryInputStream.h
    include/nucleus/Streams/OutputStream.h
    include/nucleus/Streams/StringOutputStream.h
    include/nucleus/Streams/Utils.h
    include/nucleus/Synchronization/AutoLock.h
    include/nucleus/Synchronization/Lock.h
    include/nucleus/Text/CharTraits.h
    include/nucleus/Text/DynamicString.h
    include/nucleus/Text/StaticString.h
    include/nucleus/Text/StringPool.h
    include/nucleus/Text/StringView.h
    include/nucleus/Text/Utils.h
    include/nucleus/Types.h
    include/nucleus/Win/WindowsMixin.h
    )

set(nucleus_SOURCE_FILES
    src/Debugger.cpp
    src/FilePath.cpp
    src/HighPerformanceTimer.cpp
    src/Logging.cpp
    src/Parser/Tokenizer.cpp
    src/Profiling.cpp
    src/Streams/ConsoleOutputStream.cpp
    src/Streams/ArrayInputStream.cpp
    src/Streams/DynamicBufferOutputStream.cpp
    src/Streams/FileInputStream.cpp
    src/Streams/FileOutputStream.cpp
    src/Streams/InputStream.cpp
    src/Streams/MemoryInputStream.cpp
    src/Streams/OutputStream.cpp
    src/Streams/StringOutputStream.cpp
    src/Streams/Utils.cpp
    src/Synchronization/Lock.cpp
    src/Text/DynamicString.cpp
    src/Text/StringView.cpp
    src/Text/Utils.cpp
    )

nucleus_add_library(nucleus ${nucleus_HEADER_FILES} ${nucleus_SOURCE_FILES})

find_package(Catch2 CONFIG REQUIRED)

set(nucleusTests_SOURCE_FILES
    tests/Containers/BitSetTests.cpp
    tests/Containers/DynamicArrayTests.cpp
    tests/Containers/StaticArrayTests.cpp
    tests/FilePathTests.cpp
    tests/HighPerformanceTimerTests.cpp
    tests/LoggingTests.cpp
    tests/LoggingTests.cpp
    tests/Main.cpp
    tests/Memory/ScopedPtrTests.cpp
    tests/Memory/ScopedRefPtrTests.cpp
    tests/RefCountedTests.cpp
    tests/Streams/ConsoleOutputStreamTests.cpp
    tests/Streams/StringOutputStreamTests.cpp
    tests/Text/DynamicStringTests.cpp
    tests/Text/StaticStringTests.cpp
    tests/Text/StringPoolTests.cpp
    tests/Text/StringViewTests.cpp
    tests/Text/UtilsTests.cpp
    )

nucleus_add_executable(nucleusTests ${nucleusTests_SOURCE_FILES})
target_link_libraries(nucleusTests PRIVATE nucleus Catch2::Catch2)
