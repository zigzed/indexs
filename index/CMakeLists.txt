include(../base.cmake)

set(INDEX_SRC
    bigram.h bigram.cpp
    index.cpp)

link_directories(${LIBRARY_OUTPUT_PATH})
add_library(index STATIC ${INDEX_SRC})
add_executable(test test.cpp)
target_link_libraries(test index common gtest pthread)
