cmake_minimum_required(VERSION 3.5)
project(ExtractEmail VERSION 0.0.1)
set(CMAKE_DEBUG_POSTFIX d)

add_definitions(-std=c++11)
add_definitions(-Werror) # stop compile on warning

add_executable(${PROJECT_NAME} main.cc)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} extract_lib)