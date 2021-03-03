cmake_minimum_required(VERSION 3.0)

PROJECT(MixFaceLibrary)

IF(WIN32)
 set(LIBS ${LIBS} Ws2_32 winmm)
ENDIF(WIN32)

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
    set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-mgw8-mt-d-x32-1_75.a)
    message(STATUS "Using ${Boost_LIBRARIES} libraries.")
else(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
    set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-mgw8-mt-x32-1_75.a)
    message(STATUS "Using ${Boost_LIBRARIES} libraries.")
endif(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")

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
