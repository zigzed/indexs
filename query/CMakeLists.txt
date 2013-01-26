include(../base.cmake)

set(QUERY_SRC
    query.h query.cpp)

link_directories(${LIBRARY_OUTPUT_PATH})
add_library(query STATIC ${QUERY_SRC})
add_executable(test_query test.cpp)
target_link_libraries(test_query query ngram common gtest ${THREAD} ${DL})
