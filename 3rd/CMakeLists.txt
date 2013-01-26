include(../base.cmake)

add_custom_command(OUTPUT ${LIBRARY_OUTPUT_PATH}/libkyotocabinet.a
    COMMAND ./configure && make
    COMMAND cp libkyotocabinet.a ${LIBRARY_OUTPUT_PATH}/.
    COMMAND make distclean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/kyotocabinet-1.2.76
    COMMENT "making kyotocabinet-1.2.76"
    )
add_custom_target(database ALL DEPENDS ${LIBRARY_OUTPUT_PATH}/libkyotocabinet.a)

add_custom_command(OUTPUT ${LIBRARY_OUTPUT_PATH}/libre2.a
    COMMAND make
    COMMAND cp obj/libre2.a ${LIBRARY_OUTPUT_PATH}/.
    COMMAND make distclean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/re2
    COMMENT "making re2"
    )
add_custom_target(regex ALL DEPENDS ${LIBRARY_OUTPUT_PATH}/libre2.a)
