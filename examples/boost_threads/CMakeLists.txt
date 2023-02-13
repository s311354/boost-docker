cmake_minimum_required(VERSION 3.0.0)

project(BankAccount VERSION 0.0.1 LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS thread)

set(CMAKE_CXX_STANDARD 14)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} bankaccount.cpp)
target_link_libraries(BankAccount Boost::thread)
