cmake_minimum_required(VERSION 3.10.0)

PROJECT(MixFaceLibrary)

IF(WIN32)
 set(LIBS ${LIBS} Ws2_32 winmm)
ENDIF(WIN32)

if(ANDROID)
    message(STATUS "Using ${ANDROID_ABI} libraries.")
    #if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-clang-mt-a32-1_74.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_chrono-clang-mt-a32-1_74.a)
    #elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-clang-mt-a64-1_74.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_chrono-clang-mt-a64-1_74.a)
    #elseif(${ANDROID_ABI} STREQUAL "x86")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-clang-mt-x32-1_74.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_chrono-clang-mt-x32-1_74.a)
    #elseif(${ANDROID_ABI} STREQUAL "x86_64")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-clang-mt-x64-1_74.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_chrono-clang-mt-x64-1_74.a)
    #endif()
endif()

if(WIN32)
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-mgw8-mt-d-x32-1_74.a)
    else(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread-mgw8-mt-x32-1_74.a)
    endif(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
endif()

if(UNIX)
    set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_thread.a)
    set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARY_DIRS}/libboost_chrono.a)
endif()

message(STATUS "Using ${Boost_LIBRARIES} libraries.")

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
