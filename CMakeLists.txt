cmake_minimum_required(VERSION 2.8.4)
project(TgDictionary)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(Boost_USE_MULTITHREADED ON)

add_executable(TgDictionary main.cpp)

add_subdirectory(TgCore)

target_link_libraries(TgDictionary TgCore)