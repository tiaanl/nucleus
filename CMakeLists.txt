cmake_minimum_required(VERSION 3.7.2)

if(TARGET nucleus)
    return()
endif()

project(nucleus)

# Options

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Third party libraries.

add_subdirectory("external")

# nucleus

set(nucleus_INCLUDE_FILES
    "include/nucleus/Allocators/Allocated.h"
    "include/nucleus/Allocators/Allocator.h"
    "include/nucleus/Allocators/DebugAllocator.h"
    "include/nucleus/Allocators/DefaultAllocator.h"
    "include/nucleus/Allocators/GlobalAllocator.h"
    "include/nucleus/Allocators/LinearAllocator.h"
    "include/nucleus/Allocators/TestAllocator.h"
    "include/nucleus/Config.h"
    "include/nucleus/Containers/Array.h"
    "include/nucleus/Containers/BitSet.h"
    "include/nucleus/Containers/DynamicArray.h"
    "include/nucleus/FilePath.h"
    "include/nucleus/Files.h"
    "include/nucleus/Logging.h"
    "include/nucleus/Macros.h"
    "include/nucleus/MemoryDebug.h"
    "include/nucleus/Optional.h"
    "include/nucleus/Ref.h"
    "include/nucleus/RefCounted.h"
    "include/nucleus/Signals.h"
    "include/nucleus/Streams/FileInputStream.h"
    "include/nucleus/Streams/FileOutputStream.h"
    "include/nucleus/Streams/InputStream.h"
    "include/nucleus/Streams/MemoryInputStream.h"
    "include/nucleus/Streams/OutputStream.h"
    "include/nucleus/Streams/WrappedMemoryInputStream.h"
    "include/nucleus/Synchronization/AutoLock.h"
    "include/nucleus/Synchronization/Lock.h"
    "include/nucleus/Text/String.h"
    "include/nucleus/Types.h"
    "include/nucleus/Utils/ByteOrder.h"
    "include/nucleus/Win/WindowsMixin.h"
    )

set(nucleus_SOURCE_FILES
    "src/Allocators/DebugAllocator.cpp"
    "src/Allocators/DefaultAllocator.cpp"
    "src/Allocators/GlobalAllocator.cpp"
    "src/Allocators/LinearAllocator.cpp"
    "src/FilePath.cpp"
    "src/Files.cpp"
    "src/Logging.cpp"
    "src/MemoryDebug.cpp"
    "src/Streams/FileInputStream.cpp"
    "src/Streams/FileOutputStream.cpp"
    "src/Streams/InputStream.cpp"
    "src/Streams/MemoryInputStream.cpp"
    "src/Streams/OutputStream.cpp"
    "src/Streams/WrappedMemoryInputStream.cpp"
    "src/Synchronization/Lock.cpp"
    )

if(WIN32)
    list(APPEND nucleus_SOURCE_FILES
        "src/Streams/FileInputStream_win.cpp"
        )
endif()

if(APPLE)
    list(APPEND nucleus_SOURCE_FILES
        "src/Streams/FileInputStream_posix.cpp"
        )
endif()

add_library(nucleus STATIC ${nucleus_INCLUDE_FILES} ${nucleus_SOURCE_FILES})
target_include_directories(nucleus PUBLIC "${PROJECT_SOURCE_DIR}/include")
if(MSVC)
    target_compile_options(nucleus PUBLIC "/MP")
    target_compile_definitions(nucleus PUBLIC "-D_CRTDBG_MAP_ALLOC")
endif()
set_target_properties(nucleus PROPERTIES
    CXX_STANDARD 14
    FOLDER nucleus
    )

add_subdirectory(tests "tests")
if(NOT MINGW)
    add_subdirectory(benchmarks "benchmarks")
endif()
