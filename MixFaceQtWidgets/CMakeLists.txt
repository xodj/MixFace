cmake_minimum_required(VERSION 3.0.0)

PROJECT(MixFaceQtWidgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

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
    )
set(MixFaceQtWidgets_QRC
    MixFace.qrc
    )

qt5_add_resources(MixFaceQtWidgets_QRC_SOURCES ${MixFaceQtWidgets_QRC})

add_executable(MixFaceQtWidgets
    ${MixFaceQtWidgets_SOURCES}
    ${MixFaceQtWidgets_HEADERS}
    ${MixFaceQtWidgets_QRC_SOURCES})

target_link_libraries(MixFaceQtWidgets MixFaceLibrary DebugLibrary OSCPACK Qt5::Widgets Qt5::Network)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Boost/boost_1_75_0)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DebugLibrary)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../MixFaceLibrary)
