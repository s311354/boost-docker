cmake_minimum_required(VERSION 3.5)
project(SimulatedController VERSION 0.0.1)
set(CMAKE_DEBUG_POSTFIX d)

add_definitions(-std=c++11)
add_definitions(-Werror) # stop compile on warning

find_path(INCLUDE_DIRS SimulatedController.h SerialCommunication.h HINTS ${CMAKE_SOURCE_DIR}/include)

set(SOURCE_FILES SimulatedController.cc SerialCommunication.cc)

add_library(sim_lib SHARED ${SOURCE_FILES} )
target_include_directories(sim_lib PUBLIC ${INCLUDE_DIRS})