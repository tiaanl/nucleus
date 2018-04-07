if(TARGET nucleus)
    return()
endif()

cmake_minimum_required(VERSION 3.7.2)
project(nucleus)

# Options

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Third party libraries.

add_subdirectory("external")

# nucleus

set(nucleus_HEADER_FILES
    "include/nucleus/Allocators/Allocated.h"
    "include/nucleus/Allocators/Allocator.h"
    "include/nucleus/Allocators/DebugAllocator.h"
    "include/nucleus/Allocators/DefaultAllocator.h"
    "include/nucleus/Allocators/GlobalAllocator.h"
    "include/nucleus/Allocators/LinearAllocator.h"
    "include/nucleus/Allocators/TestAllocator.h"
    "include/nucleus/ByteOrder.h"
    "include/nucleus/Config.h"
    "include/nucleus/Containers/Array.h"
    "include/nucleus/Containers/BitSet.h"
    "include/nucleus/Containers/DynamicArray.h"
    "include/nucleus/FilePath.h"
    "include/nucleus/Files.h"
    "include/nucleus/HighPerformanceTimer.h"
    "include/nucleus/Logging.h"
    "include/nucleus/Macros.h"
    "include/nucleus/Memory/Ptr.h"
    "include/nucleus/MemoryDebug.h"
    "include/nucleus/MessageLoop/MessageLoop.h"
    "include/nucleus/MessageLoop/MessagePump.h"
    "include/nucleus/MessageLoop/MessagePumpDefault.h"
    "include/nucleus/MessageLoop/Task.h"
    "include/nucleus/Optional.h"
    "include/nucleus/Ref.h"
    "include/nucleus/RefCounted.h"
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
    "include/nucleus/Win/WindowsMixin.h"

    "include/nucleus/Callbacks/Bind.h"
    "include/nucleus/Callbacks/BindDetail.h"
    "include/nucleus/Callbacks/BindState.h"
    "include/nucleus/Callbacks/Callback.h"
    "include/nucleus/Callbacks/CallbackDetail.h"
    "include/nucleus/Callbacks/CallbackForward.h"
    "include/nucleus/Callbacks/ExtractReturnType.h"
    "include/nucleus/Callbacks/FunctorTraits.h"
    "include/nucleus/Callbacks/MakeBindStateType.h"
    "include/nucleus/Callbacks/MakeFunctionType.h"
    "include/nucleus/Callbacks/PassingTraits.h"
    "include/nucleus/Callbacks/TypeList.h"
    )

set(nucleus_SOURCE_FILES
    "src/Allocators/DebugAllocator.cpp"
    "src/Allocators/DefaultAllocator.cpp"
    "src/Allocators/GlobalAllocator.cpp"
    "src/Allocators/LinearAllocator.cpp"
    "src/FilePath.cpp"
    "src/Files.cpp"
    "src/HighPerformanceTimer.cpp"
    "src/Logging.cpp"
    "src/MessageLoop/MessageLoop.cpp"
    "src/MessageLoop/MessagePump.cpp"
    "src/MessageLoop/MessagePumpDefault.cpp"
    "src/Streams/FileInputStream.cpp"
    "src/Streams/FileOutputStream.cpp"
    "src/Streams/InputStream.cpp"
    "src/Streams/MemoryInputStream.cpp"
    "src/Streams/OutputStream.cpp"
    "src/Streams/WrappedMemoryInputStream.cpp"
    "src/Synchronization/Lock.cpp"

    "src/Callbacks/CallbackDetail.cpp"
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
    target_compile_options(nucleus PUBLIC /W4 /MP)

    # Enable memory debugging.
    target_compile_definitions(nucleus PUBLIC "-D_CRTDBG_MAP_ALLOC")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    # Enable warnings.
    target_compile_options(nucleus PUBLIC -Wall -Wextra -pedantic)
endif()

set_target_properties(nucleus PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER nucleus
    )

add_subdirectory(tests "tests")
if(NOT MINGW)
    add_subdirectory(benchmarks "benchmarks")
endif()
