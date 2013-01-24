include(../base.cmake)

set(INDEX_SRC
    bigram.h bigram.cpp
    index.h index.cpp)

include_directories(${INDEXS_PATH}/3rd/kyotocabinet-1.2.76/)
link_directories(${LIBRARY_OUTPUT_PATH})
add_library(index STATIC ${INDEX_SRC})
add_executable(test test.cpp)
target_link_libraries(test index kyotocabinet common gtest ${THREAD} ${DL} z)
