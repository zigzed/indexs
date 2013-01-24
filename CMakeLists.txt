project(indexs CXX C)
cmake_minimum_required(VERSION 2.8)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0015 NEW)
endif(COMMAND cmake_policy)

if(WIN32)
    add_definitions("/W3 /wd4996 /wd4290")
endif(WIN32)

include(base.cmake)
add_subdirectory(index)
add_subdirectory(query)

