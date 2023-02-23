
cmake_minimum_required(VERSION 3.11)
project(tinyxml2-test)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#set(tinyxml2_DIR /home/qiangwang/xml2bip/cmake)

set(SRC_LIST src/xmltest.cpp src/tinyxml2.cpp)

enable_testing()

#find_package(tinyxml2 REQUIRED)
#include_directories(include)

add_executable(xml2smv ${SRC_LIST})
#target_link_libraries(xml2smv PUBLIC tinyxml2::tinyxml2)
