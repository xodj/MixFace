cmake_minimum_required(VERSION 3.0)

PROJECT(MixFaceLibrary)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

IF(WIN32)
 set(LIBS ${LIBS} Ws2_32 winmm)
ENDIF(WIN32)

set(MixFaceLibrary_HEADERS
MixFaceKeeper.h
MixFaceLibrary.h
MixFaceLinker.h
MixFaceListener.h
)

set(MixFaceLibrary_SOURCES
MixFaceKeeper.cpp
MixFaceLibrary.cpp
MixFaceLinker.cpp
)

ADD_LIBRARY(MixFaceLibrary
        ${MixFaceLibrary_SOURCES}
        ${MixFaceLibrary_HEADERS}
)

target_link_libraries(MixFaceLibrary DebugLibrary OSCPACK Qt5::Widgets ${LIBS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Boost/boost_1_75_0)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DebugLibrary)
