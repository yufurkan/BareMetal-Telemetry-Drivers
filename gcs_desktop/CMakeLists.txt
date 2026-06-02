cmake_minimum_required(VERSION 3.16)
project(GroundControlStation VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add qt modules 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets SerialPort REQUIRED)

# source files
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    ui/mainwindow.ui
)


add_executable(GroundControlStation ${PROJECT_SOURCES})

# add Qt libraries into project 
target_link_libraries(GroundControlStation PRIVATE Qt6::Widgets Qt6::SerialPort)
target_include_directories(GroundControlStation PRIVATE include)