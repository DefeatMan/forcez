cmake_minimum_required(VERSION 3.24)

option(USE_WINDOWS "use mingw compile" ON)

if(USE_WINDOWS)
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/mingw_x86_64_toolchain.cmake)
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

project(forcez LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(BIT7Z_AUTO_FORMAT)

include(FetchContent)

FetchContent_Declare(
  bit7z
  GIT_REPOSITORY https://github.com/rikyoz/bit7z.git
  GIT_TAG v4.0.9
)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.3
)

FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  GIT_TAG v3.2
)

set(build_static_lib ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(bit7z spdlog argparse)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cc)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE spdlog forcezlib)
