cmake_minimum_required(VERSION 3.10.0)

project(MixFace)

OPTION(DISABLE_QTWIDGETS "Disable QT build" OFF)
OPTION(DISABLE_WXWIDGETS "Disable WX build" ON)
OPTION(DISABLE_X32ST "Disable shutdown tool" ON)

set(COMPILER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MinGW-W64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin")
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Boost/boost_1_74_0")

set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}")
set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib")

OPTION(OSC_LIB_ONLY "Enable build only library" ON)
add_subdirectory(OSCPACK)

add_subdirectory(DebugLibrary)
add_subdirectory(MixFaceLibrary)

IF(NOT DISABLE_WXWIDGETS)
add_subdirectory(MixFaceWXWidgets)
ENDIF(NOT DISABLE_WXWIDGETS)

IF(NOT DISABLE_QTWIDGETS)
add_subdirectory(MixFaceQtWidgets)
ENDIF(NOT DISABLE_QTWIDGETS)

IF(NOT DISABLE_X32ST)
add_subdirectory(X32-Shutdown)
ENDIF(NOT DISABLE_X32ST)

#add_subdirectory(nana)
#add_subdirectory(nanogui)
