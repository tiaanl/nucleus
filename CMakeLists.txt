cmake_minimum_required(VERSION 3.7.2)
project("nucleus")

#General options

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Compiler options

include("cmake/gcc.cmake")
include("cmake/msvc.cmake")
include("cmake/win32.cmake")

#Third party libraries.

include("cmake/gtest.cmake")

#nucleus

include_directories("include")

set("nucleus_INCLUDE_FILES"
        "include/nucleus/config.h"

        "include/nucleus/Allocators/Allocator.h"

        "include/nucleus/Atomics/Atomics.h"
        "include/nucleus/Atomics/AtomicRefCount.h"

        "include/nucleus/Containers/array.h"
        "include/nucleus/Containers/Vector.h"
        "include/nucleus/files/file_path.h"
        "include/nucleus/files/file_utils.h"
        "include/nucleus/logging.h"
        "include/nucleus/macros.h"

        "include/nucleus/Memory/RefCounted.h"
        "include/nucleus/Memory/ScopedPtr.h"
        "include/nucleus/Memory/ScopedRefPtr.h"

        "include/nucleus/streams/file_input_stream.h"
        "include/nucleus/streams/input_stream.h"
        "include/nucleus/streams/memory_input_stream.h"
        "include/nucleus/streams/wrapped_memory_input_stream.h"
        "include/nucleus/strings/char_ptr.h"
        "include/nucleus/strings/char_ptr_traits_16.h"
        "include/nucleus/strings/char_ptr_traits_32.h"
        "include/nucleus/strings/char_ptr_traits_8.h"
        "include/nucleus/strings/char_ptr_traits_a.h"
        "include/nucleus/strings/char_ptr_traits_w.h"
        "include/nucleus/strings/String.h"
        "include/nucleus/strings/string_utf_conversion.h"
        "include/nucleus/strings/string_utils.h"
        "include/nucleus/synchronization/auto_lock.h"
        "include/nucleus/synchronization/lock.h"
        "include/nucleus/types.h"

        "include/nucleus/utils/CommonType.h"
        "include/nucleus/utils/Copy.h"
        "include/nucleus/utils/Functional.h"
        "include/nucleus/utils/Memory.h"
        "include/nucleus/utils/Move.h"
        "include/nucleus/utils/TypeTraits.h"

        "include/nucleus/utils/byte_order.h"
        "include/nucleus/utils/detail/observer_list_base.h"
        "include/nucleus/utils/observer_list.h"
        "include/nucleus/utils/signals.h"
        "include/nucleus/utils/stl.h"


        "include/nucleus/win/windows_mixin.h"
        )

set("nucleus_SOURCE_FILES"
        "src/files/file_path.cpp"
        "src/logging.cpp"

        "src/Allocators/Allocator.cpp"

        "src/Memory/RefCounted.cpp"

        "src/streams/file_input_stream.cpp"
        "src/streams/input_stream.cpp"
        "src/streams/memory_input_stream.cpp"
        "src/streams/wrapped_memory_input_stream.cpp"
        "src/strings/String.cpp"
        "src/strings/string_utf_conversion.cpp"
        "src/strings/string_utils.cpp"
        )

if (WIN32)
    list(APPEND
            "nucleus_SOURCE_FILES"
            "src/files/file_utils_win.cpp"
            "src/streams/file_input_stream_win.cpp"
            "src/synchronization/lock_win32.cpp"
            )
endif ()

if (APPLE)
    list(APPEND
            "nucleus_SOURCE_FILES"
            "src/streams/file_input_stream_posix.cpp"
            )
endif ()

set("nucleus_TEST_FILES"
        # "tests/Atomics/AtomicsTests.cpp"

        "tests/Allocators/AllocatorTests.cpp"

        "tests/Memory/RefCountedTests.cpp"
        "tests/Memory/ScopedPtrTests.cpp"

        "tests/containers/array_tests.cpp"
        "tests/containers/vector_tests.cpp"
        "tests/files/file_path_tests.cpp"
        "tests/logging_test.cpp"
        "tests/strings/char_ptr_tests.cpp"
        "tests/strings/string_utf_conversion_tests.cpp"
        "tests/strings/string_utils_tests.cpp"
        "tests/utils/observer_list_tests.cpp"
        "tests/utils/signals_tests.cpp"
        )

add_library("nucleus" STATIC ${nucleus_INCLUDE_FILES} ${nucleus_SOURCE_FILES})
target_include_directories("nucleus" PUBLIC "${nucleus_SOURCE_DIR}/include")
set_property(TARGET "nucleus" PROPERTY FOLDER "nucleus")

add_executable("nucleus_tests" ${nucleus_TEST_FILES})
target_link_libraries("nucleus_tests" "gtest" "gmock" "gmock_main" "nucleus")
set_property(TARGET "nucleus_tests" PROPERTY FOLDER "nucleus")
