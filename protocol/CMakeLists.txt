cmake_minimum_required(VERSION 3.25)

find_package(Protobuf REQUIRED CONFIG)

protobuf_generate_cpp(
    PROTO_SRCS 
    PROTO_HDRS 
    ${CMAKE_CURRENT_LIST_DIR}/job.proto
    )

qt_add_library(protocol STATIC
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    protocol.h
    protocol.cpp
    )

target_link_libraries(protocol
    PRIVATE
        ${Protobuf_LIBRARIES}
        Qt6::Core
        Qt6::Gui
        Qt6::Mqtt
    )

target_include_directories(protocol
    PUBLIC 
        ${Protobuf_INCLUDE_DIRS}
    )

include(GNUInstallDirs)
install(TARGETS protocol
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# TODO: Add tests here!
