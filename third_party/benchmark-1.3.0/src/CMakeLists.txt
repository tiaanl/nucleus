
file(GLOB
  SOURCE_FILES
    *.cc
    ${PROJECT_SOURCE_DIR}/include/benchmark/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

if (DEFINED BENCHMARK_CXX_LINKER_FLAGS)
  list(APPEND CMAKE_SHARED_LINKER_FLAGS ${BENCHMARK_CXX_LINKER_FLAGS})
  list(APPEND CMAKE_MODULE_LINKER_FLAGS ${BENCHMARK_CXX_LINKER_FLAGS})
endif()

add_library(benchmark ${SOURCE_FILES})
target_include_directories(benchmark PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(benchmark PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)

# Link threads.
find_library(LIBRT rt)
if(LIBRT)
  target_link_libraries(benchmark ${LIBRT})
endif()

# We need extra libraries on Windows.
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(benchmark Shlwapi)
endif()
