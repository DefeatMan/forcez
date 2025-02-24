aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} FORCEZ_SRC_LIST)

add_library(forcezlib STATIC ${FORCEZ_SRC_LIST})

target_link_libraries(forcezlib bit7z argparse)

target_include_directories(forcezlib PRIVATE ${CMAKE_SOURCE_DIR}/include)
