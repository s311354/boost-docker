cmake_minimum_required(VERSION 3.5)

project(Dead_Lock VERSION 0.0.1 LANGUAGES CXX)

# Find the pthread library
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} dead_lock.cpp)

# Add the -g flag to include debugging information
set_target_properties(Dead_Lock PROPERTIES COMPILE_FLAGES "-g -O2 -Wall -pthread")
target_link_libraries(${PROJECT_NAME} Threads::Threads)
