
cmake_minimum_required(VERSION 3.15)
project(tinyxml2-test)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

enable_testing()

find_package(tinyxml2 REQUIRED)
include_directories(./include)

add_executable(xml2smv src/xmltest.cpp)
target_link_libraries(xml2smv PUBLIC tinyxml2::tinyxml2)
