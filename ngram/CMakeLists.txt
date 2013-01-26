include(../base.cmake)

set(NGRAM_SRC
    bigram.h bigram.cpp
    idxdbi.h idxdbi.cpp
    )

include_directories(${INDEXS_PATH}/3rd/kyotocabinet-1.2.76/)
link_directories(${LIBRARY_OUTPUT_PATH})
add_library(ngram STATIC ${NGRAM_SRC})
target_link_libraries(ngram kyotocabinet z)
add_executable(test_ngram test.cpp)
target_link_libraries(test_ngram ngram common gtest ${THREAD} ${DL})
