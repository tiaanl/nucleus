cmake_minimum_required(VERSION 3.16)

if (TARGET nucleus)
    return()
endif ()

project(nucleus)

option(NUCLEUS_SKIP_TESTS "Do not build tests." OFF)

find_package(Catch2 CONFIG REQUIRED)

include(cmake/utils.cmake)

set(HEADER_FILES
    include/nucleus/byte_order.h
    include/nucleus/config.h
    include/nucleus/containers/array_view.h
    include/nucleus/containers/bit_set.h
    include/nucleus/containers/dynamic_array.h
    include/nucleus/containers/static_array.h
    include/nucleus/containers/hash_map.h
    include/nucleus/containers/hash_table.h
    include/nucleus/containers/hash_table_base.h
    include/nucleus/containers/sparse_array.h
    include/nucleus/containers/stable_pool.h
    include/nucleus/debugger.h
    include/nucleus/file_path.h
    include/nucleus/high_resolution_timer.h
    include/nucleus/logging.h
    include/nucleus/macros.h
    include/nucleus/memory/ref_counted_ptr.h
    include/nucleus/memory/scoped_ptr.h
    include/nucleus/memory/scoped_ref_ptr.h
    include/nucleus/parser/tokenizer.h
    include/nucleus/profiling.h
    include/nucleus/ref_counted.h
    include/nucleus/source_location.h
    include/nucleus/streams/array_input_stream.h
    include/nucleus/streams/console_output_stream.h
    include/nucleus/streams/dynamic_buffer_output_stream.h
    include/nucleus/streams/file_input_stream.h
    include/nucleus/streams/file_output_stream.h
    include/nucleus/streams/input_stream.h
    include/nucleus/streams/memory_input_stream.h
    include/nucleus/streams/output_stream.h
    include/nucleus/streams/string_output_stream.h
    include/nucleus/streams/utils.h
    include/nucleus/synchronization/auto_lock.h
    include/nucleus/synchronization/lock.h
    include/nucleus/text/char_traits.h
    include/nucleus/text/dynamic_string.h
    include/nucleus/text/static_string.h
    include/nucleus/text/string_pool.h
    include/nucleus/text/string_view.h
    include/nucleus/text/utils.h
    include/nucleus/types.h
    include/nucleus/win/includes.h
    include/nucleus/function.h
    include/nucleus/hash.h
    include/nucleus/main_header.hpp
    include/nucleus/message_loop/message_loop.h
    include/nucleus/message_loop/message_pump.h
    include/nucleus/optional.h
    include/nucleus/testing/lifetime_tracker.h
    include/nucleus/threading/scoped_thread_local_ptr.h
    include/nucleus/threading/thread.h
    include/nucleus/threading/thread_local.h
    include/nucleus/type_id.h
    )

set(SOURCE_FILES
    src/debugger.cpp
    src/file_path.cpp
    src/high_resolution_timer.cpp
    src/logging.cpp
    src/parser/tokenizer.cpp
    src/profiling.cpp
    src/streams/array_input_stream.cpp
    src/streams/console_output_stream.cpp
    src/streams/dynamic_buffer_output_stream.cpp
    src/streams/file_input_stream.cpp
    src/streams/file_output_stream.cpp
    src/streams/input_stream.cpp
    src/streams/memory_input_stream.cpp
    src/streams/output_stream.cpp
    src/streams/string_output_stream.cpp
    src/streams/Utils.cpp
    src/synchronization/lock.cpp
    src/text/dynamic_string.cpp
    src/text/string_view.cpp
    src/text/utils.cpp
    src/message_loop/message_loop.cpp
    src/message_loop/message_pump.cpp
    src/testing/lifetime_tracker.cpp
    src/threading/thread.cpp
    src/threading/thread_local.cpp
    )

nucleus_add_library(nucleus ${HEADER_FILES} ${SOURCE_FILES})

# TODO: Only if posix
if (WIN32)
else ()
    target_link_libraries(nucleus PRIVATE pthread)
endif ()

nucleus_add_library(tests_main
    tests/tests_main.cpp
    )
target_link_libraries(tests_main PUBLIC Catch2::Catch2)

if (NOT ${NUCLEUS_SKIP_TESTS})
    set(TEST_FILES
        tests/containers/bit_set_tests.cpp
        tests/containers/dynamic_array_tests.cpp
        tests/containers/static_array_tests.cpp
        tests/containers/hash_map_tests.cpp
        tests/containers/hash_table_base_tests.cpp
        tests/containers/hash_table_tests.cpp
        tests/containers/sparse_array_tests.cpp
        tests/containers/stable_pool_tests.cpp
        tests/file_path_tests.cpp
        tests/high_resolution_timer_tests.cpp
        tests/logging_tests.cpp
        tests/logging_tests.cpp
        "tests/memory/scoped_ptr_tests.cpp"
        "tests/memory/scoped_ref_ptr_tests.cpp"
        tests/ref_counted_tests.cpp
        tests/streams/console_output_stream_tests.cpp
        tests/streams/string_output_stream_tests.cpp
        tests/text/dynamic_string_tests.cpp
        tests/text/static_string_tests.cpp
        tests/text/string_pool_tests.cpp
        tests/text/string_view_tests.cpp
        tests/text/utils_tests.cpp
        tests/byte_order_tests.cpp
        tests/function_tests.cpp
        tests/hash_tests.cpp
        tests/message_loop/message_loop_tests.cpp
        tests/optional_tests.cpp
        tests/threading/scoped_thread_local_ptr_tests.cpp
        tests/threading/thread_local_tests.cpp
        tests/threading/thread_tests.cpp
        )

    nucleus_add_executable(nucleus_tests ${TEST_FILES})
    target_link_libraries(nucleus_tests PRIVATE nucleus tests_main)
endif ()
