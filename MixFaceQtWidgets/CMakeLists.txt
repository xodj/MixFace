cmake_minimum_required(VERSION 3.0.0)

PROJECT(MixFaceQtWidgets LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Widgets)
find_package(Qt5Network)

set(MixFaceQtWidgets_HEADERS
    MixFaceWindow.h
    MixFaceFaderWidget.h
    MixFaceSourceWidget.h
    MixFaceDynamicsWidget.h
    MixFaceEqualizerWidget.h
    MixFaceFonts.h
    MixFaceEnumTypes.h
    MixFaceStaticMath.h
    MixFaceIconPicker.h
    MixFaceRightWidget.h
    MixFaceTopWidget.h
    MixFaceVolumeMeter.h
    )
set(MixFaceQtWidgets_SOURCES
    MixFaceWindow.cpp
    MixFaceFaderWidget.cpp
    MixFaceSourceWidget.cpp
    MixFaceDynamicsWidget.cpp
    MixFaceEqualizerWidget.cpp
    MixFaceIconPicker.cpp
    MixFaceRightWidget.cpp
    MixFaceTopWidget.cpp
    MixFaceVolumeMeter.cpp
    )
set(MixFaceQtWidgets_QRC
    MixFaceResources.qrc
    )

qt5_add_resources(MixFaceQtWidgets_QRC_SOURCES ${MixFaceQtWidgets_QRC})

if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/android CACHE INTERNAL "")
    add_library(MixFaceQtWidgets SHARED
        ${MixFaceQtWidgets_SOURCES}
        ${MixFaceQtWidgets_HEADERS}
        ${MixFaceQtWidgets_QRC_SOURCES}
        android/AndroidManifest.xml)
    target_link_libraries(MixFaceQtWidgets MixFaceLibrary_${ANDROID_ABI} DebugLibrary_${ANDROID_ABI} OSCPACK_${ANDROID_ABI} Qt5::Widgets Qt5::Network)
else()
    add_executable(MixFaceQtWidgets
        ${MixFaceQtWidgets_SOURCES}
        ${MixFaceQtWidgets_HEADERS}
        ${MixFaceQtWidgets_QRC_SOURCES})
    target_link_libraries(MixFaceQtWidgets MixFaceLibrary DebugLibrary OSCPACK Qt5::Widgets Qt5::Network)
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Boost/boost_1_75_0)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DebugLibrary)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../MixFaceLibrary)
