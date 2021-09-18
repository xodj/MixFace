cmake_minimum_required(VERSION 3.16.0)

PROJECT(MixFaceLibrary VERSION 2.1.0.114 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

if(ANDROID)
    message(STATUS "Using ${ANDROID_ABI} libraries.")
    if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-clang-mt-a32-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-clang-mt-a32-1_76.a)
    elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-clang-mt-a64-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-clang-mt-a64-1_76.a)
    elseif(${ANDROID_ABI} STREQUAL "x86")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-clang-mt-x32-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-clang-mt-x32-1_76.a)
    elseif(${ANDROID_ABI} STREQUAL "x86_64")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-clang-mt-x64-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-clang-mt-x64-1_76.a)
    endif()
endif()

if(WIN32 AND NOT MSVC)
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-mgw8-mt-d-x64-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-mgw8-mt-d-x64-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_date_time-mgw8-mt-d-x64-1_76.a)
    else(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-mgw8-mt-x64-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-mgw8-mt-x64-1_76.a)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_date_time-mgw8-mt-x64-1_76.a)
    endif(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
endif()

if(MSVC)
    string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
    if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-vc142-mt-gd-x64-1_76.lib)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-vc142-mt-gd-x64-1_76.lib)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_date_time-vc142-mt-gd-x64-1_76.lib)
    else(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread-vc142-mt-x64-1_76.lib)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono-vc142-mt-x64-1_76.lib)
        set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_date_time-vc142-mt-x64-1_76.lib)
    endif(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR uppercase_CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
endif()

if(UNIX AND NOT ANDROID)
    set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_thread.a)
    set(Boost_LIBRARIES ${Boost_LIBRARIES} libboost_chrono.a)
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
endif()

if(NOT UNIX)
    set_target_properties(MixFaceLibrary PROPERTIES PREFIX "")
    set_target_properties(MixFaceLibrary PROPERTIES OUTPUT_NAME "MixFaceLibrary")
endif()

if(WIN32 AND MINGW)
    target_link_libraries(MixFaceLibrary PUBLIC DebugLibrary PRIVATE ${Boost_LIBRARIES} OSCPACK)
else()
    target_link_libraries(MixFaceLibrary PUBLIC DebugLibrary ${Boost_LIBRARIES} PRIVATE OSCPACK)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
