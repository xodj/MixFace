cmake_minimum_required(VERSION 3.16.0)

PROJECT(MixFace VERSION 0.2.0.148 LANGUAGES CXX)

if(UNIX)
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

set(MixFaceQtWidgets_HEADERS
    MixFaceWindow.h
    MixFaceFaderWidget.h
    MixFaceSourceWidget.h
    MixFaceDynamicsWidget.h
    MixFaceEqualizerWidget.h
    MixFaceFonts.h
    MixFaceEnumTypes.h
    MixFaceStaticMath.h
    MixFaceIconPicker.h
    MixFaceRightWidget.h
    MixFaceTopWidget.h
    MixFaceVolumeMeter.h
    )
set(MixFaceQtWidgets_SOURCES
    MixFaceWindow.cpp
    MixFaceFaderWidget.cpp
    MixFaceSourceWidget.cpp
    MixFaceDynamicsWidget.cpp
    MixFaceEqualizerWidget.cpp
    MixFaceIconPicker.cpp
    MixFaceRightWidget.cpp
    MixFaceTopWidget.cpp
    MixFaceVolumeMeter.cpp
    )
set(MixFaceQtWidgets_QRC
    MixFaceResources.qrc
    )

if(WIN32)
    #Place version to .rc
    set(STR_MF_VERSION ${MixFace_VERSION})
    string(REPLACE "." "." RC_SPLIT ${STR_MF_VERSION})
    string(REPLACE "." ";" RC_SPLIT ${RC_SPLIT})
    message(STATUS "MixFace is now: ${STR_MF_VERSION}")
    list(GET RC_SPLIT 0 MF_VERSION)
    list(GET RC_SPLIT 1 MF_MAJOR_VERSION)
    list(GET RC_SPLIT 2 MF_MINOR_VERSION)
    list(GET RC_SPLIT 3 MF_PATCH_VERSION)
    configure_file(MixFace.rc.in ${PROJECT_BINARY_DIR}/MixFace.rc)
    set(MixFaceQtWidgets_HEADERS
        ${MixFaceQtWidgets_HEADERS}
        MixFace.rc.in
        ${PROJECT_BINARY_DIR}/MixFace.rc
        )
endif(WIN32)

qt_add_resources(MixFaceQtWidgets_QRC_SOURCES ${MixFaceQtWidgets_QRC})

if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/android CACHE INTERNAL "")
    add_library(MixFace SHARED
        ${MixFaceQtWidgets_SOURCES}
        ${MixFaceQtWidgets_HEADERS}
        ${MixFaceQtWidgets_QRC_SOURCES}
        android/AndroidManifest.xml.in
        android/AndroidManifest.xml)
    #Place version to .xml
    set(STR_MF_VERSION ${MixFace_VERSION})
    string(REPLACE "." "." RC_SPLIT ${STR_MF_VERSION})
    string(REPLACE "." ";" RC_SPLIT ${RC_SPLIT})
    message(STATUS "MixFace is now: ${STR_MF_VERSION}")
    list(GET RC_SPLIT 0 MF_VERSION)
    list(GET RC_SPLIT 1 MF_MAJOR_VERSION)
    list(GET RC_SPLIT 2 MF_MINOR_VERSION)
    list(GET RC_SPLIT 3 MF_PATCH_VERSION)
    configure_file(${PROJECT_SOURCE_DIR}/android/AndroidManifest.xml.in ${PROJECT_SOURCE_DIR}/android/AndroidManifest.xml)
endif()

if(NOT ANDROID)
    add_executable(MixFace
        ${MixFaceQtWidgets_SOURCES}
        ${MixFaceQtWidgets_HEADERS}
        ${MixFaceQtWidgets_QRC_SOURCES})

set_target_properties(MixFace
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
endif()

target_link_libraries(MixFace PRIVATE MixFaceLibrary DebugLibrary Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

if(WIN32 AND MSVC)
set_target_properties(MixFace PROPERTIES
    LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
    LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()

if(WIN32 AND MINGW)
    set_target_properties(MixFace PROPERTIES
        LINK_FLAGS_RELEASE "-mwindows"
        LINK_FLAGS_MINSIZEREL "-mwindows"
        LINK_FLAGS_RELWITHDEBINFO "-mwindows"
        )
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../MixFaceLibrary)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DebugLibrary)
