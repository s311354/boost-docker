cmake_minimum_required(VERSION 3.5)
project(ExtractEmail VERSION 0.0.1)
set(CMAKE_DEBUG_POSTFIX d)

add_definitions(-std=c++11)
add_definitions(-Werror) # stop compile on warning

find_path(INCLUDE_DIRS extractemail.h HINTS ${CMAKE_SOURCE_DIR}/include)

set(SOURCE_FILES extractemail.cc)

add_library(extract_lib SHARED ${SOURCE_FILES} )
target_include_directories(extract_lib PUBLIC ${INCLUDE_DIRS})