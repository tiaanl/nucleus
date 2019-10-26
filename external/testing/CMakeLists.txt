
set(testing_HEADER_FILES
    include/nucleus/Testing.h
    )

set(testing_SOURCE_FILES
    src/Testing.cpp
    )

nucleus_add_library(testing ${testing_HEADER_FILES} ${testing_SOURCE_FILES})
