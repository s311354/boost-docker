cmake_minimum_required(VERSION 3.0.0)

project(BankAccount VERSION 0.0.1 LANGUAGES CXX)

add_definitions(-DLOCK_GUARD_LOCKABLE_ADAPTER)

find_package(Boost REQUIRED COMPONENTS thread)

set(CMAKE_CXX_STANDARD 20)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} bankaccount.cpp)
target_link_libraries(BankAccount Boost::thread)

# Add the -g flag to include debugging information
set_target_properties(BankAccount PROPERTIES COMPILE_FLAGES "-g -O2")
