cmake_minimum_required(VERSION 3.27)
project(ImageViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(plugins/ErodePlugin)
add_subdirectory(plugins/SharpenPlugin)
add_subdirectory(plugins/RotatePlugin)
add_subdirectory(plugins/CartoonifyPlugin)
add_subdirectory(plugins/AffinePlugin)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(ImageViewer main.cpp
        Classes/Header/MainWindow.h
        Classes/Source/MainWindow.cpp
        Classes/Header/EditorPluginInterface.h)

target_link_libraries(ImageViewer
        PRIVATE ErodePlugin
        PRIVATE SharpenPlugin
        PRIVATE RotatePlugin
        PRIVATE CartoonifyPlugin
        PRIVATE AffinePlugin
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${OpenCV_LIBS}

)

