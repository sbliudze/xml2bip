
cmake_minimum_required(VERSION 3.11)
project(tinyxml2-test)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# Enabling C++11 (C++0x) in CMake
# https://www.guyrutenberg.com/2014/01/05/enabling-c11-c0x-in-cmake/

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#set(tinyxml2_DIR /home/qiangwang/xml2bip/cmake)

set(SRC_LIST src/xmltest.cpp src/tinyxml2.cpp)

enable_testing()

#find_package(tinyxml2 REQUIRED)
#include_directories(include)

add_executable(xml2smv ${SRC_LIST})
#target_link_libraries(xml2smv PUBLIC tinyxml2::tinyxml2)
