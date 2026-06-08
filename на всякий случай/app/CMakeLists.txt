cmake_minimum_required(VERSION 3.16)

project(app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Sql Network)

qt_standard_project_setup(REQUIRES 6.8)

# Заголовочные файлы
set(HEADERS
    include/dbworker.h
    include/logs.h
    include/tcpclient.h
    include/tcpserver.h
    include/settings.h
    include/exceptions.h
)

# Исходные файлы C++
set(SOURCES
    src/dbworker.cpp
    src/logs.cpp
    src/tcpclient.cpp
    src/tcpserver.cpp
    src/settings.cpp
    src/exceptions.cpp
    src/main.cpp
)

# QML файлы
set(QML_FILES
    resources/qml/Main.qml
    resources/qml/LoginPage.qml
    resources/qml/OrderPage.qml
    resources/qml/StockPage.qml
    resources/qml/ReportsPage.qml
)

qt_add_executable(appapp ${SOURCES})

target_sources(appapp PRIVATE ${HEADERS})

qt_add_resources(appapp "app_icons"
    PREFIX "/"
    FILES
        resources/icons/logo.png
)

# QML модуль (включает ресурсы)
qt_add_qml_module(appapp
    URI app
    VERSION 1.0
    QML_FILES ${QML_FILES}
    QML_FILES resources/qml/ActiveOrdersPage.qml
)

target_link_libraries(appapp
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Network
)

target_include_directories(appapp PRIVATE include)

set_target_properties(appapp PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

include(GNUInstallDirs)
install(TARGETS appapp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

enable_testing()
add_subdirectory(tests)
