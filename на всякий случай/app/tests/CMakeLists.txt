cmake_minimum_required(VERSION 3.16)

project(tst_test LANGUAGES CXX)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Core Test Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)          # обязательно для поиска заголовков
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к исходникам основного проекта
set(APP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(APP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Исходники приложения
set(APP_SOURCES
    ${APP_SRC_DIR}/logs.cpp
    ${APP_SRC_DIR}/settings.cpp
    ${APP_SRC_DIR}/tcpclient.cpp
    ${APP_SRC_DIR}/exceptions.cpp            # обязательно
)

# Заголовки приложения – нужны для генерации moc
set(APP_HEADERS
    ${APP_INCLUDE_DIR}/logs.h
    ${APP_INCLUDE_DIR}/settings.h
    ${APP_INCLUDE_DIR}/tcpclient.h
    ${APP_INCLUDE_DIR}/exceptions.h
)

# Файлы тестов
set(TEST_SOURCES
    unit/main.cpp
    unit/test_logs.cpp
    unit/test_settings.cpp
    unit/test_tcpclient.cpp
)

set(TEST_HEADERS
    unit/test_logs.h
    unit/test_settings.h
    unit/test_tcpclient.h
)

# Исполняемый файл тестов: включаем исходники и заголовки
add_executable(tst_test
    ${TEST_SOURCES}
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${TEST_HEADERS}
)

target_include_directories(tst_test PRIVATE ${APP_INCLUDE_DIR})

target_link_libraries(tst_test PRIVATE Qt6::Test Qt6::Core Qt6::Network)

add_test(NAME tst_test COMMAND tst_test)
