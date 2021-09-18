cmake_minimum_required(VERSION 3.16.0)

PROJECT(DebugLibrary VERSION 1.2.0.53 LANGUAGES CXX)

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

if(UNIX AND NOT ANDROID)
    add_library(DebugLibrary STATIC
            ${DebugLibrary_SOURCES}
            )
else(UNIX AND NOT ANDROID)
    add_library(DebugLibrary SHARED
            ${DebugLibrary_SOURCES}
            )
endif(UNIX AND NOT ANDROID)

if(NOT UNIX)
    set_target_properties(DebugLibrary
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
    set_target_properties(DebugLibrary PROPERTIES PREFIX "")
    set_target_properties(DebugLibrary PROPERTIES OUTPUT_NAME "DebugLibrary")
endif()

if(ANDROID)
    set_target_properties(DebugLibrary
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/")
    set_target_properties(DebugLibrary PROPERTIES OUTPUT_NAME "DebugLibrary_${ANDROID_ABI}")
endif()
