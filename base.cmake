set(ROOT_PATH ${CMAKE_SOURCE_DIR}/../)
set(COMMON_PATH	${ROOT_PATH}/common)
set(INDEXS_PATH ${ROOT_PATH}/indexs)

set(LIBRARY_OUTPUT_PATH	    ${INDEXS_PATH}/bin/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH  ${INDEXS_PATH}/bin/${CMAKE_BUILD_TYPE})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(THREAD  pthread)
    set(DL      dl)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(${COMMON_PATH})
link_directories(${COMMON_PATH}/lib/${CMAKE_BUILD_TYPE})
