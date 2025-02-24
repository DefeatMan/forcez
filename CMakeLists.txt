cmake_minimum_required(VERSION 3.24)

option(USE_WINDOWS "use mingw compile" ON)

if(USE_WINDOWS)
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/mingw_x86_64_toolchain.cmake)
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

project(forcez LANGUAGES CXX)

add_compile_definitions(BIT7Z_AUTO_FORMAT)
add_definitions(-DELPP_THREAD_SAFE -DELPP_NO_DEFAULT_LOG_FILE)

include(FetchContent)

FetchContent_Declare(
  bit7z
  GIT_REPOSITORY https://github.com/rikyoz/bit7z.git
  GIT_TAG v4.0.9
)

FetchContent_Declare(
  easyloggingpp
  GIT_REPOSITORY https://github.com/amrayn/easyloggingpp.git
  GIT_TAG v9.97.1
)

FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  GIT_TAG v3.2
)

set(build_static_lib ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(bit7z easyloggingpp argparse)

add_executable(forcez main.cc)

target_include_directories(forcez PRIVATE ${easyloggingpp_SOURCE_DIR}/src)

target_link_libraries(forcez bit7z easyloggingpp argparse)
