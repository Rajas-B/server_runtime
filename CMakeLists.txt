cmake_minimum_required(VERSION 3.15)
project(ReactorRuntime VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(reactor_lib
    src/reactor/Engine.cpp
    src/reactor/Reactor.cpp
    src/reactor/ReactorEventContext.cpp
    src/reactor/WakeupHandler.cpp
)

target_include_directories(reactor_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/reactor
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/reactor
)

add_subdirectory(apps)