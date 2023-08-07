cmake_minimum_required(VERSION 3.16)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(jobhandler
    main.cpp
    worker.h
    worker.cpp
)

target_link_libraries(jobhandler
    PRIVATE 
        Qt6::Core
        Qt6::Quick
        Qt6::Mqtt
        Qt6::Concurrent
        protocol
        configuration
)

include(GNUInstallDirs)
install(TARGETS jobhandler
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET jobhandler
    OUTPUT_SCRIPT jobhandler_deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${jobhandler_deploy_script})

# TODO: Add tests here!
