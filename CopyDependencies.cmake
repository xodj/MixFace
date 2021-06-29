if(NOT WIN32)
        return()
endif()

# Internal variable to avoid copying more than once
if(COPIED_DEPENDENCIES)
	return()
endif()

option(COPY_DEPENDENCIES "Automatically try copying all dependencies" ON)
if(NOT COPY_DEPENDENCIES)
	return()
endif()

#COPY DEPS
message("\nCopy Dependencies...\n")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

set(COPIED_DEPENDENCIES TRUE CACHE BOOL "Dependencies have been copied, set to false to copy again" FORCE)

if(NOT CMAKE_BUILD_TYPE MATCHES "Debug")

configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Core.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Core.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Gui.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Gui.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Widgets.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Widgets.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Network.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Network.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/plugins/platforms/qwindows.dll ${PROJECT_BINARY_DIR}/Appfolder/Platforms/qwindows.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/plugins/styles/qwindowsvistastyle.dll ${PROJECT_BINARY_DIR}/Appfolder/Styles/qwindowsvistastyle.dll COPYONLY)

else(NOT CMAKE_BUILD_TYPE MATCHES "Debug")

configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Cored.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Cored.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Guid.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Guid.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Networkd.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Networkd.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/bin/Qt${QT_VERSION_MAJOR}Widgetsd.dll ${PROJECT_BINARY_DIR}/Appfolder/Qt${QT_VERSION_MAJOR}Widgetsd.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/plugins/platforms/qwindowsd.dll ${PROJECT_BINARY_DIR}/Appfolder/Platforms/qwindowsd.dll COPYONLY)
configure_file(${CMAKE_PREFIX_PATH}/plugins/styles/qwindowsvistastyled.dll ${PROJECT_BINARY_DIR}/Appfolder/Styles/qwindowsvistastyled.dll COPYONLY)

endif(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
