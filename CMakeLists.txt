cmake_minimum_required(VERSION 3.22.1)
project(DecantTool)

add_executable(DecantTool ./src/main.cpp)
target_include_directories(DecantTool PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/reqs")