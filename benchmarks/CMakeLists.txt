
set(nucleusBenchmarks_HEADER_FILES)

set(nucleusBenchmarks_SOURCE_FILES
    "src/Allocators/AllocatorBenchmarks.cpp"
    )

add_executable(nucleusBenchmarks ${nucleusBenchmarks_HEADER_FILES} ${nucleusBenchmarks_SOURCE_FILES})
target_link_libraries(nucleusBenchmarks nucleus benchmark)
set_target_properties(nucleusBenchmarks PROPERTIES
    CXX_STANDARD 14
    FOLDER nucleus
    )
if(MSVC)
    target_compile_options(nucleusBenchmarks PUBLIC "/MP")
    target_compile_definitions(nucleusBenchmarks PUBLIC "-D_CRTDBG_MAP_ALLOC")
endif()
