cmake_minimum_required(VERSION 3.0)

PROJECT(DebugLibrary)

ADD_LIBRARY(DebugLibrary DebugLibrary.hpp DebugLibrary.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../Boost/boost_1_75_0)
