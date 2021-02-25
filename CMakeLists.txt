cmake_minimum_required(VERSION 3.0.0)

project(MixFace)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
find_package(Qt5Network)

OPTION(OSC_LIB_ONLY "Enable build only library" ON)
add_subdirectory(OSCPACK)

add_subdirectory(DebugLibrary)

add_subdirectory(MixFaceLibrary)
add_subdirectory(MixFaceQtWidgets)
#add_subdirectory(MixFaceWXWidgets)
