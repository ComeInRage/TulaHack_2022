cmake_minimum_required(VERSION 2.8.4)
project(TgCore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_subdirectory(tgbot-cpp)

set(CORE_FILES json.hpp
               curl_request.cpp
               File.cpp
               Application.cpp
               Translator.cpp)

add_library(TgCore STATIC ${CORE_FILES})

target_link_libraries(TgCore TgBot ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})
