
set(testing_HEADER_FILES
    include/nucleus/Testing.h
    include/nucleus/DynamicArrayTesting.h
    )

set(testing_SOURCE_FILES
    src/Testing.cpp
    )

nucleus_add_library(testing ${testing_HEADER_FILES} ${testing_SOURCE_FILES})
target_link_libraries(testing catch2)
