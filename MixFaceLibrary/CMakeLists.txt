cmake_minimum_required(VERSION 3.0)
PROJECT(MixFaceLibrary)

set(MixFaceLibrary_HEADERS
MixFaceKeeper.h
MixFaceLibrary.h
MixFaceLinker.h
MixFaceListener.h
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
