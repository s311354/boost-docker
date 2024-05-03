cmake_minimum_required(VERSION 3.5)
project(DeadLock VERSION 0.0.1)
set(CMAKE_DEBUG_POSTFIX d)

add_definitions(-std=c++11)
add_definitions(-Werror) # stop compile on warning

find_path(INCLUDE_DIRS deadlock.h HINTS ${CMAKE_SOURCE_DIR}/include)

set(SOURCE_FILES deadlock.cc)

add_library(deadlock_lib SHARED ${SOURCE_FILES} )
target_include_directories(deadlock_lib PUBLIC ${INCLUDE_DIRS})