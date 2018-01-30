
set(testing_HEADER_FILES "include/nucleus/Testing.h")
set(testing_SOURCE_FILES "src/Testing.cpp")

add_library(testing ${testing_HEADER_FILES} ${testing_SOURCE_FILES})
target_link_libraries(testing catch2)
target_include_directories(testing PUBLIC "include")
set_target_properties(testing PROPERTIES FOLDER "external")
