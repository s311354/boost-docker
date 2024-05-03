cmake_minimum_required(VERSION 3.5)
project(SimulatedController VERSION 0.0.1)
set(CMAKE_DEBUG_POSTFIX d)

add_definitions(-std=c++11)
add_definitions(-Werror) # stop compile on warning
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/include)

add_subdirectory(src)
add_subdirectory(impl)