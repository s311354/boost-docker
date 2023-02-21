cmake_minimum_required(VERSION 3.0.0)

project(BankAccount VERSION 0.0.1 LANGUAGES CXX)

# External and internal locking approach 
#add_definitions(-DEXTERNAL_INTERNAL_LOCK)
# Caller-ensured locking approach
# add_definitions(-DCALLER_ENSURED_LOCK)
# Guard's constructor locking approach
# add_definitions(-DLOCK_GUARD)
# Internal locking approach

# Runtime check throw if not locks the smae
#add_definitions(-DBOOST_THREAD_THROW_IF_PRECONDITION_NOT_SATISFIED)

find_package(Boost REQUIRED COMPONENTS thread)

set(CMAKE_CXX_STANDARD 14)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} bankaccount.cpp)
target_link_libraries(BankAccount Boost::thread)

# Add the -g flag to include debugging information
set_target_properties(BankAccount PROPERTIES COMPILE_FLAGES "-g -O2 -Wall -pthread")
