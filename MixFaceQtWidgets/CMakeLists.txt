cmake_minimum_required(VERSION 3.16.0)

PROJECT(MixFace VERSION 0.2.2.228 LANGUAGES CXX)

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

set(Qt6_FOUND TRUE)
set(Qt6Network_FOUND TRUE)

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
    #Place version to .xml
    set(STR_MF_VERSION ${MixFace_VERSION})
    string(REPLACE "." "." RC_SPLIT ${STR_MF_VERSION})
    string(REPLACE "." ";" RC_SPLIT ${RC_SPLIT})
    message(STATUS "MixFace is now: ${STR_MF_VERSION}")
    list(GET RC_SPLIT 0 MF_VERSION)
    list(GET RC_SPLIT 1 MF_MAJOR_VERSION)
    list(GET RC_SPLIT 2 MF_MINOR_VERSION)
    list(GET RC_SPLIT 3 MF_PATCH_VERSION)

    configure_file(${PROJECT_SOURCE_DIR}/AndroidManifest.xml.in
        ${PROJECT_SOURCE_DIR}/android/AndroidManifest.xml)
    configure_file(${PROJECT_SOURCE_DIR}/android-MixFace-deployment-settings.json.in
        ${CMAKE_BINARY_DIR}/android-MixFace-deployment-settings.json)

    set(ANDROID_PACKAGE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/android CACHE INTERNAL "")
    add_library(MixFace SHARED
        ${MixFaceQtWidgets_SOURCES}
        ${MixFaceQtWidgets_HEADERS}
        ${MixFaceQtWidgets_QRC_SOURCES}
        )
    set_target_properties(MixFace PROPERTIES OUTPUT_NAME "MixFace_${ANDROID_ABI}")

    ADD_CUSTOM_COMMAND(TARGET MixFace POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/MixFaceQtWidgets/libMixFace_${ANDROID_ABI}.so
        ${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libMixFace_${ANDROID_ABI}.so
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/DebugLibrary/libDebugLibrary_${ANDROID_ABI}.so
        ${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libDebugLibrary_${ANDROID_ABI}.so
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/MixFaceLibrary/libMixFaceLibrary_${ANDROID_ABI}.so
        ${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libMixFaceLibrary_${ANDROID_ABI}.so
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/OSCPACK/libOSCPACK_${ANDROID_ABI}.so
        ${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libOSCPACK_${ANDROID_ABI}.so
        COMMENT "Copy Libraries To Gradle...")
else()
    add_executable(MixFace
        ${MixFaceQtWidgets_SOURCES}
        ${MixFaceQtWidgets_HEADERS}
        ${MixFaceQtWidgets_QRC_SOURCES})
    set_target_properties(MixFace
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Appfolder")
endif()

target_link_libraries(MixFace PRIVATE
    MixFaceLibrary
    DebugLibrary
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network)

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
    include(CopyDependencies.cmake)
endif()

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
