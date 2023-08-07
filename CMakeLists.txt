cmake_minimum_required(VERSION 3.25)

project(root)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui Core Network Mqtt Concurrent)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(config)
add_subdirectory(protocol)
add_subdirectory(jobemu)
add_subdirectory(jobhandler)
#add_subdirectory(client)
#add_subdirectory(server)
