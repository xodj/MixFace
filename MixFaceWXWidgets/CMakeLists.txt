cmake_minimum_required(VERSION 3.5)

project(MixFaceWXWidgets)

set(wxWidgets_EXCLUDE_COMMON_LIBRARIES true)
find_package(wxWidgets REQUIRED richtext xrc aui html adv core xml net base scintilla tiff jpeg png zlib regex expat)
include(${wxWidgets_USE_FILE})

set(MixFaceWXWidgets_HEADERS
    MainFrame.h
    wxcrafter.h
    dbMeter.h
)

set(MixFaceWXWidgets_SOURCES
    main.cpp
    MainFrame.cpp
    wxcrafter_bitmaps.cpp
    dbMeter.cpp
)

if(WIN32)
	set(SRCS ${SRCS} win_resources.rc)
endif(WIN32)

add_executable(MixFaceWXWidgets
        ${MixFaceWXWidgets_SOURCES}
        ${MixFaceWXWidgets_HEADERS}
		${SRCS}
)

target_link_libraries(MixFaceWXWidgets
        ${wxWidgets_LIBRARIES}
		)
