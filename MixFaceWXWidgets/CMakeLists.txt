cmake_minimum_required(VERSION 3.0)

project(MixFaceWXWidgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

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

set(wxWidgets_EXCLUDE_COMMON_LIBRARIES true)
find_package(wxWidgets REQUIRED richtext xrc aui html adv core xml net base scintilla tiff jpeg png zlib regex expat)
include(${wxWidgets_USE_FILE})

set(MixFaceWXWidgets_HEADERS
    MixFaceWindow.h
    MixFaceWindowBase.h
    dbMeter.h
    MixFaceFader.h
    MixFaceFaderBase.h
)

set(MixFaceWXWidgets_SOURCES
    main.cpp
    MixFaceWindow.cpp
    MixFaceWindowBase.cpp
    MixFaceWindowBitmaps.cpp
    dbMeter.cpp
    MixFaceFader.cpp
    MixFaceFaderBase.cpp
    MixFaceFaderBitmaps.cpp
)


set(WXCP MixFaceWindow.wxcp)

if(WIN32)
        set(SRCS ${SRCS}
            MixFaceWindowBitmaps.xrc
            MixFaceFaderBitmaps.xrc
            )
endif(WIN32)

add_executable(MixFaceWXWidgets
        ${MixFaceWXWidgets_SOURCES}
        ${MixFaceWXWidgets_HEADERS}
        ${SRCS}
        ${WXCP}
        )

target_link_libraries(MixFaceWXWidgets
        ${wxWidgets_LIBRARIES}
        DebugLibrary
        MixFaceLibrary
        OSCPACK
        ${LIBS}
        ${Boost_LIBRARIES}
        )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DebugLibrary)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../MixFaceLibrary)

