cmake_minimum_required(VERSION 3.16)

add_library(configuration STATIC
    configuration.h
    configuration.cpp
)

target_include_directories(configuration
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}
    )
