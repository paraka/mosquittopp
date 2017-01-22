cmake_minimum_required (VERSION 2.8.11)
project(Moquittopp_client)

# For mosquito library
include_directories("/usr/local/include")

set(CMAKE_CXX_FLAGS "-std=c++14 -g")

find_package(Threads)

set(SRCS MQTTClient.cpp main.cpp)
add_executable(mosquitto_client ${SRCS})
target_link_libraries(mosquitto_client mosquittopp ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS mosquitto_client DESTINATION bin)
