cmake_minimum_required(VERSION 3.16)
project(RaftKV)

include_directories(./include ./modules)

aux_source_directory(src SOURCES_FILES)

add_subdirectory(modules)

set(CMAKE_CXX_STANDARD 14)

add_executable(RaftKV main.cpp ${SOURCES_FILES})

target_link_libraries(RaftKV Modules)
