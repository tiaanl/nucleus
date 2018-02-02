
# Benchmark
if(NOT MINGW)
    add_subdirectory("benchmark-1.3.0")
endif()

# Testing
add_subdirectory("catch2-2.1.1")
add_subdirectory("testing")
