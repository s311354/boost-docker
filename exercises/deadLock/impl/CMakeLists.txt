cmake_minimum_required(VERSION 3.5)
project(DeadLock VERSION 0.0.1)
set(CMAKE_DEBUG_POSTFIX d)

# Find the pthread library
find_package(Threads REQUIRED)

add_definitions(-std=c++11)
add_definitions(-Werror) # stop compile on warning

add_executable(${PROJECT_NAME} main.cc)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} deadlock_lib Threads::Threads)