include(GNUInstallDirs)

macro(nucleus_set_flags target)
    if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        # Enable warnings and multi-process compilation.
        target_compile_options(${target} PUBLIC /W3 /WX /MP /WX)

        # Enable memory debugging.
        target_compile_definitions(${target} PUBLIC "-D_CRTDBG_MAP_ALLOC")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        # Enable warnings.
        target_compile_options(${target} PUBLIC -Wall -Wextra -pedantic)  # -Werror
    endif ()
endmacro(nucleus_set_flags)

macro(nucleus_set_properties target header_files)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        FOLDER ${CMAKE_PROJECT_NAME}
        DEBUG_POSTFIX _d
        )

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    install(TARGETS ${target}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

    foreach (file ${header_files})
        get_filename_component(dir ${file} DIRECTORY)
        install(FILES ${file} DESTINATION ${dir})
    endforeach()
endmacro(nucleus_set_properties)

function(nucleus_add_library target)
    set(multi HEADER_FILES SOURCE_FILES)
    cmake_parse_arguments(PARSED_ARGS "" "" "${multi}" ${ARGN})

    add_library(${target} ${PARSED_ARGS_HEADER_FILES} ${PARSED_ARGS_SOURCE_FILES})

    target_include_directories(${target} PUBLIC include)

    nucleus_set_flags(${target})
    nucleus_set_properties(${target} "${PARSED_ARGS_HEADER_FILES}")
endfunction(nucleus_add_library)

function(nucleus_add_executable name)
    set(multi HEADER_FILES SOURCE_FILES)
    cmake_parse_arguments(PARSED_ARGS "" "" "${multi}" ${ARGN})

    add_executable(${name} ${PARSED_ARGS_HEADER_FILES} ${PARSED_ARGS_SOURCE_FILES})

    nucleus_set_flags(${name})
    nucleus_set_properties(${name} "${PARSED_ARGS_HEADER_FILES}")
endfunction(nucleus_add_executable)
