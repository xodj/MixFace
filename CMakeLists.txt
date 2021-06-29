cmake_minimum_required(VERSION 3.16.0)

project(MixFace)

OPTION(DISABLE_QTWIDGETS "Disable QT build" OFF)
OPTION(DISABLE_WXWIDGETS "Disable WX build" ON)
OPTION(DISABLE_X32ST "Disable shutdown tool" ON)

if(NOT BOOST_ROOT)
    message(WARNING "Please set BOOST_ROOT path")
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../boost_1_74_0")
    message(STATUS "Default BOOST_ROOT path is ${BOOST_ROOT}")
endif()

if(ANDROID)
    set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/stage/include/boost-1_74")
else()
    set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}")
endif()
set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib")

message(STATUS "BOOST_LIBRARY_DIRS is ${BOOST_LIBRARY_DIRS}")
message(STATUS "BOOST_INCLUDE_DIRS is ${BOOST_INCLUDE_DIRS}")

OPTION(OSC_LIB_ONLY "Enable build only library" ON)
add_subdirectory(OSCPACK)
add_subdirectory(DebugLibrary)
add_subdirectory(MixFaceLibrary)

if(NOT ANDROID)
    set_target_properties(OSCPACK
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
    set_target_properties(DebugLibrary
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
    set_target_properties(MixFaceLibrary
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
endif()

if(NOT DISABLE_QTWIDGETS)
add_subdirectory(MixFaceQtWidgets)
if(NOT ANDROID)
set_target_properties(MixFaceQtWidgets
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
include(CopyDependencies.cmake)
endif()
endif(NOT DISABLE_QTWIDGETS)

if(NOT DISABLE_X32ST)
add_subdirectory(X32-Shutdown)
set_target_properties(X32-Shutdown
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
endif(NOT DISABLE_X32ST)
