cmake_minimum_required(VERSION 3.10.0)

PROJECT(DebugLibrary)


if(ANDROID)
    ADD_LIBRARY(DebugLibrary
    #ADD_LIBRARY(DebugLibrary_${ANDROID_ABI} SHARED
        DebugLibrary.hpp
        DebugLibrary.cpp
        )
else()
    ADD_LIBRARY(DebugLibrary DebugLibrary.hpp DebugLibrary.cpp)
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
