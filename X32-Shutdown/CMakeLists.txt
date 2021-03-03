cmake_minimum_required(VERSION 3.0.0)

PROJECT(X32-Shutdown)

IF(WIN32)
 set(LIBS ${LIBS} Ws2_32 winmm)
ENDIF(WIN32)

add_executable(X32-Shutdown main.cpp)

target_link_libraries(X32-Shutdown OSCPACK ${LIBS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../OSCPACK)
