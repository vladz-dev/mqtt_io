cmake_minimum_required(VERSION 3.16)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(clientapp
    resources.qrc
    main.cpp
)

target_link_libraries(clientapp
    PRIVATE 
        Qt6::Core
        Qt6::Quick
        protocol

)

include(GNUInstallDirs)
install(TARGETS clientapp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET clientapp
    OUTPUT_SCRIPT clientapp_deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${clientapp_deploy_script})
