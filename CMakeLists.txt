cmake_minimum_required(VERSION 3.24)

option(USE_WINDOWS "use mingw compile" ON)

if(USE_WINDOWS)
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/mingw_x86_64_toolchain.cmake)
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(VERSION_FILE_PATH "${CMAKE_SOURCE_DIR}/version.txt")

if (EXISTS ${VERSION_FILE_PATH})
  file(READ ${VERSION_FILE_PATH} PROJECT_VERSION_FROM_FILE)
  string(STRIP ${PROJECT_VERSION_FROM_FILE} PROJECT_VERSION_FROM_FILE)
else()
  set(PROJECT_VERSION_FROM_FILE "0.0.0")
  message(WARNING "version.txt file not found, using default version ${PROJECT_VERSION_FROM_FILE}")
endif()

project(forcez
  VERSION ${PROJECT_VERSION_FROM_FILE}
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(GEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/gen")

set(CONFIG_H_OUTPUT "${GEN_OUTPUT_DIR}/config.h")

configure_file(
  config.h.in
  ${CONFIG_H_OUTPUT}
  @ONLY
)

add_compile_definitions(BIT7Z_AUTO_FORMAT)

include(FetchContent)

FetchContent_Declare(
  bit7z
  GIT_REPOSITORY https://github.com/rikyoz/bit7z.git
  GIT_TAG v4.0.9
)

FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  GIT_TAG v3.2
)

set(build_static_lib ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(bit7z argparse)

### Libs ###
aux_source_directory(${CMAKE_SOURCE_DIR}/src/forcez FORCEZ_LIBS)

add_library(forcezlib STATIC ${FORCEZ_LIBS})

target_include_directories(forcezlib PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(forcezlib bit7z argparse)

### App ###
add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cc)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GEN_OUTPUT_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE forcezlib)
