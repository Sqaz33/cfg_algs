cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(cfg_algs)

file(GLOB SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE "src")

add_subdirectory("src/graphviz")

target_link_libraries(${PROJECT_NAME} PRIVATE graphviz_wrapper)