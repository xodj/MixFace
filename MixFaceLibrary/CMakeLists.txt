cmake_minimum_required(VERSION 3.16.0)

PROJECT(MixFaceLibrary VERSION 2.1.1.125 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

message(STATUS "Using ${Boost_LIBRARIES} libraries.")

set(MixFaceLibrary_HEADERS
    MixFaceStatic.h
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

if(WIN32)
    #Place version to .rc
    set(STR_MFL_VERSION ${MixFaceLibrary_VERSION})
    string(REPLACE "." "." RC_SPLIT ${STR_MFL_VERSION})
    string(REPLACE "." ";" RC_SPLIT ${RC_SPLIT})
    message(STATUS "MixFaceLibrary is now: ${STR_MFL_VERSION}")
    list(GET RC_SPLIT 0 MFL_VERSION)
    list(GET RC_SPLIT 1 MFL_MAJOR_VERSION)
    list(GET RC_SPLIT 2 MFL_MINOR_VERSION)
    list(GET RC_SPLIT 3 MFL_PATCH_VERSION)
    configure_file(MixFaceLibrary.rc.in ${PROJECT_BINARY_DIR}/MixFaceLibrary.rc)
    set(MixFaceLibrary_HEADERS
        ${MixFaceLibrary_HEADERS}
        MixFaceLibrary.rc.in
        ${PROJECT_BINARY_DIR}/MixFaceLibrary.rc
        )
endif(WIN32)

if(UNIX AND NOT ANDROID)
    add_library(MixFaceLibrary STATIC
            ${MixFaceLibrary_SOURCES}
            ${MixFaceLibrary_HEADERS})
else(UNIX AND NOT ANDROID)
    add_library(MixFaceLibrary SHARED
            ${MixFaceLibrary_SOURCES}
            ${MixFaceLibrary_HEADERS})
endif(UNIX AND NOT ANDROID)

if(ANDROID)
    set_target_properties(MixFaceLibrary
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/")
    set_target_properties(MixFaceLibrary PROPERTIES OUTPUT_NAME "MixFaceLibrary_${ANDROID_ABI}")
else()
    set_target_properties(MixFaceLibrary
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
    if(NOT UNIX)
        set_target_properties(MixFaceLibrary PROPERTIES PREFIX "")
        set_target_properties(MixFaceLibrary PROPERTIES OUTPUT_NAME "MixFaceLibrary")
    endif()
endif()


if(MINGW)
    target_link_libraries(MixFaceLibrary PUBLIC DebugLibrary PRIVATE ${Boost_LIBRARIES} OSCPACK)
else()
    target_link_libraries(MixFaceLibrary PUBLIC DebugLibrary ${Boost_LIBRARIES} PRIVATE OSCPACK)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
