cmake_minimum_required(VERSION 3.19)
project(PhotoLab VERSION 0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

# Файлы проекта
file(GLOB TOOLS_SRC Filters/*.cpp)
file(GLOB CNN_SRC CNN/*.cpp)
file(GLOB CONTROLLER_SRC controller/*.cpp)
file(GLOB GUI_SRC gui/*.cpp)

# Библиотека
add_library(controller STATIC
    ${CONTROLLER_SRC}
    ${CNN_SRC}
    ${TOOLS_SRC}
)

target_compile_options(controller PRIVATE --coverage)
target_link_options(controller PRIVATE --coverage)

target_include_directories(controller PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(Program
    main.cpp
    gui/mainwindow.cpp
    include/gui/mainwindow.h
    gui/mainwindow.ui
)

target_include_directories(Program PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(Program PRIVATE
    controller
    Qt6::Widgets
    Qt6::Charts
)

target_compile_options(Program PRIVATE --coverage)
target_link_options(Program PRIVATE --coverage)

# ============================
# === Тестирование с GTest ===
# ============================

find_package(GTest REQUIRED)
include(GoogleTest)

enable_testing()

add_executable(TestPhotoLab
    tests/CNN_kernel_tests.cpp

)

target_link_libraries(TestPhotoLab PRIVATE
    controller
    GTest::gtest_main
)

gtest_discover_tests(TestPhotoLab)

target_compile_options(TestPhotoLab PRIVATE --coverage)
target_link_options(TestPhotoLab PRIVATE --coverage)