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

macro(nucleus_set_properties target)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        FOLDER ${CMAKE_PROJECT_NAME}
        )

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endmacro(nucleus_set_properties)

function(nucleus_add_library target)
    add_library(${target} ${ARGN})

    target_include_directories(${target} PUBLIC include)

    nucleus_set_flags(${target})
    nucleus_set_properties(${target})
endfunction(nucleus_add_library)

function(nucleus_add_executable target)
    add_executable(${target} ${ARGN})

    nucleus_set_flags(${target})
    nucleus_set_properties(${target})
endfunction(nucleus_add_executable)
