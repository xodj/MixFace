cmake_minimum_required(VERSION 3.0.0)

project(MixFace)

set(COMPILER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MinGW-W64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin")
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Boost/boost_1_75_0")
set(QT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../Qt/5.15.2/mingw81_32")

set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}")
set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib")

set(CMAKE_PREFIX_PATH "${QT_DIR}")
set(QT_QMAKE_EXECUTABLE "${QT_DIR}/bin/qmake.exe")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
find_package(Qt5Network)

OPTION(OSC_LIB_ONLY "Enable build only library" ON)
add_subdirectory(OSCPACK)

add_subdirectory(DebugLibrary)

add_subdirectory(MixFaceLibrary)
add_subdirectory(MixFaceQtWidgets)
add_subdirectory(MixFaceWXWidgets)
