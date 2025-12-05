cmake_minimum_required(VERSION 3.10)

project(Untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Game src/main.cpp)

# set(CMAKE_EXE_LINKER_FLAGS "-static")
# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(BUILD_SHARED_LIBS OFF)

target_link_libraries(
    Game PRIVATE m raylib
)

target_compile_options(
    Game PRIVATE -O3
)