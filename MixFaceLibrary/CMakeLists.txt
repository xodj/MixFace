cmake_minimum_required(VERSION 3.0)

PROJECT(MixFaceLibrary)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

IF(WIN32)
 set(LIBS ${LIBS} Ws2_32 winmm)
ENDIF(WIN32)

set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../Boost/boost_1_75_0")
set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}")
set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib")
if(CMAKE_BUILD_TYPE="DEBUG")
    set(LIBS ${LIBS} ${BOOST_LIBRARY_DIRS}/libboost_thread-mgw8-mt-d-x32-1_75.a)
else()
    set(LIBS ${LIBS} ${BOOST_LIBRARY_DIRS}/libboost_thread-mgw8-mt-x32-1_75.a)
endif(CMAKE_BUILD_TYPE="DEBUG")

set(MixFaceLibrary_HEADERS
MixFaceKeeper.h
MixFaceLibrary.h
MixFaceLinker.h
MixFaceListener.h
Timer.hpp
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

target_link_libraries(MixFaceLibrary DebugLibrary OSCPACK ${LIBS} ${Boost_LIBRARIES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DebugLibrary)
