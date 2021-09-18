cmake_minimum_required(VERSION 3.16.0)

project(MixFaceProject)

#Boost library finder
if(ANDROID)
    set(Boost_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Boost-for-Android/build/out/${ANDROID_ABI}/include/boost-1_76/")
    set(Boost_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Boost-for-Android/build/out/${ANDROID_ABI}/lib/")
else()
    set(Boost_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../boost_1_76_0/")
    set(Boost_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../boost_1_76_0/stage/lib/")
endif()
message(STATUS "BOOST LIBRARY DIR is ${Boost_LIBRARY_DIR}")
message(STATUS "BOOST INCLUDE DIR is ${Boost_INCLUDE_DIR}")
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

#Options
OPTION(DISABLE_QTWIDGETS "Disable QT build" OFF)
OPTION(DISABLE_X32ST "Disable shutdown tool" ON)

#OSC ONLY LIBRARY BUILD
OPTION(OSC_LIB_ONLY "Enable build only library" ON)

#SUBDIRS
add_subdirectory(OSCPACK)
include_directories(OSCPACK)
add_subdirectory(DebugLibrary)
include_directories(DebugLibrary)
add_subdirectory(MixFaceLibrary)
include_directories(MixFaceLibrary)

#GUI
if(NOT DISABLE_QTWIDGETS)
    add_subdirectory(MixFaceQtWidgets)
    include_directories(MixFaceQtWidgets)
endif(NOT DISABLE_QTWIDGETS)

#TOOL
if(NOT DISABLE_X32ST)
add_subdirectory(X32-Shutdown)
endif(NOT DISABLE_X32ST)
