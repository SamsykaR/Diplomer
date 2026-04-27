cmake_minimum_required(VERSION 3.16)

project(app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appapp
    main.cpp
)

qt_add_qml_module(appapp
    URI app
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES
        QML_FILES OrderPage.qml
        QML_FILES StockPage.qml
        QML_FILES ReportsPage.qml
        QML_FILES LoginPage.qml
        SOURCES dbworker.h dbworker.cpp
        SOURCES logs.h logs.cpp
        SOURCES
        SOURCES tcpclient.h tcpclient.cpp
        SOURCES
        SOURCES settings.h settings.cpp
        SOURCES tcpserver.h tcpserver.cpp
        SOURCES exceptions.h exceptions.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appapp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appapp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appapp
    PRIVATE Qt6::Quick
    Qt::Sql
)

include(GNUInstallDirs)
install(TARGETS appapp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
