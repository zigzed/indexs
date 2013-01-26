include(../base.cmake)

set(INDEX_SRC
    index.h index.cpp)

link_directories(${LIBRARY_OUTPUT_PATH})
add_library(index STATIC ${INDEX_SRC})
add_executable(test_index test.cpp)
target_link_libraries(test_index index ngram common gtest ${THREAD} ${DL})
