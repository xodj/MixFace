cmake_minimum_required(VERSION 3.16.0)

PROJECT(DebugLibrary VERSION 1.2.0.81 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)
endif()

set(DebugLibrary_SOURCES
    DebugLibrary.hpp
    DebugLibrary.cpp
    )

if(WIN32)
    #Place version to .rc
    set(STR_DL_VERSION ${DebugLibrary_VERSION})
    string(REPLACE "." "." RC_SPLIT ${STR_DL_VERSION})
    string(REPLACE "." ";" RC_SPLIT ${RC_SPLIT})
    message(STATUS "DebugLibrary is now: ${STR_DL_VERSION}")
    list(GET RC_SPLIT 0 DL_VERSION)
    list(GET RC_SPLIT 1 DL_MAJOR_VERSION)
    list(GET RC_SPLIT 2 DL_MINOR_VERSION)
    list(GET RC_SPLIT 3 DL_PATCH_VERSION)
    configure_file(DebugLibrary.rc.in ${PROJECT_BINARY_DIR}/DebugLibrary.rc)
    set(DebugLibrary_SOURCES
        ${DebugLibrary_SOURCES}
        DebugLibrary.rc.in
        ${PROJECT_BINARY_DIR}/DebugLibrary.rc
        )
endif(WIN32)

if(ANDROID)
    add_library(DebugLibrary STATIC
        ${DebugLibrary_SOURCES}
        )
else()
    add_library(DebugLibrary SHARED
        ${DebugLibrary_SOURCES}
        )
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
