cmake_minimum_required(VERSION 3.16)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(jobemu
    resources.qrc
    main.cpp
    uicore.cpp
    uicore.h
    emulator.cpp
    emulator.h
    logger.cpp
    logger.h
)

target_link_libraries(jobemu
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Mqtt
        protocol
        configuration
)

include(GNUInstallDirs)
install(TARGETS jobemu
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET jobemu
    OUTPUT_SCRIPT jobemu_deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${jobemu_deploy_script})
