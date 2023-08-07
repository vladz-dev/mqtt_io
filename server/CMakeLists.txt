cmake_minimum_required(VERSION 3.16)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(server
    main.cpp
)

target_link_libraries(server
    PRIVATE 
        Qt6::Core
        Qt6::Network
        protocol
)

include(GNUInstallDirs)
install(TARGETS server
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET jobhandler
    OUTPUT_SCRIPT server_deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${server_deploy_script})

