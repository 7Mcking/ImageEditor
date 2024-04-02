# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.27)

project(CartoonifyPlugin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core
        Gui
        Widgets REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(
        ${OpenCV_INCLUDE_DIRS}
)
set(CMAKE_CXX_STANDARD 17)

add_library(CartoonifyPlugin SHARED
        CartoonifyPlugin.cpp
        CartoonifyPlugin.h)

target_include_directories(CartoonifyPlugin PUBLIC .)

target_link_libraries(CartoonifyPlugin
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${OpenCV_LIBS})