
if(MSVC)

  message(STATUS "MSVC: Enable multi-processor compilation.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

endif()
