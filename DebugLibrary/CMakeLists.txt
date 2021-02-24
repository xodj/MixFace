cmake_minimum_required(VERSION 2.6)
PROJECT(DebugLibrary)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(DebugLibrary DebugLibrary.h)
