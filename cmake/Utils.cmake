
function(nucleus_add_library name)
    add_library(${name} ${ARGN})

    target_include_directories(${name} PUBLIC include)

    if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        # Enable warnings and multi-process compilation.
        target_compile_options(${name} PUBLIC /W3 /WX /MP /WX)

        # Enable memory debugging.
        target_compile_definitions(${name} PUBLIC "-D_CRTDBG_MAP_ALLOC")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        # Enable warnings.
        target_compile_options(${name} PUBLIC -Wall -Wextra -pedantic -Werror)
    endif ()

    set_target_properties(${name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        FOLDER ${CMAKE_PROJECT_NAME}
        )

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endfunction(nucleus_add_library)

function(nucleus_add_executable name)
    add_executable(${name} ${ARGN})

    if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        # Enable warnings and multi-process compilation.
        target_compile_options(${name} PUBLIC /W3 /WX /MP /WX)

        # Enable memory debugging.
        target_compile_definitions(${name} PUBLIC "-D_CRTDBG_MAP_ALLOC")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        # Enable warnings.
        target_compile_options(${name} PUBLIC -Wall -Wextra -pedantic -Werror)
    endif ()

    set_target_properties(${name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        FOLDER ${CMAKE_PROJECT_NAME}
        )

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endfunction(nucleus_add_executable)
