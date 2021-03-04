cmake_minimum_required(VERSION 3.0)

project(MixFaceWXWidgets)

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
    MixFaceBaseObjects.h
    MixFaceResources.h
    MixFaceMeter.h
    MixFaceFader.h
)

set(MixFaceWXWidgets_SOURCES
    MixFace.cpp
    MixFaceBaseObjects.cpp
    MixFaceResources.cpp
    MixFaceResourcesBitmaps.cpp
    MixFaceMeter.cpp
    MixFaceFader.cpp
)

if(WIN32)
        set(SRCS ${SRCS}
            MixFaceBaseObjects.fbp
            MixFaceResources.wxcp
            )
endif(WIN32)

add_executable(MixFaceWXWidgets
        ${MixFaceWXWidgets_SOURCES}
        ${MixFaceWXWidgets_HEADERS}
        ${SRCS}
        )

target_link_libraries(MixFaceWXWidgets
        ${wxWidgets_LIBRARIES}
        DebugLibrary
        #MixFaceLibrary
        #OSCPACK
        #${LIBS}
        ${Boost_LIBRARIES}
        )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DebugLibrary)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../MixFaceLibrary)

