cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_BUILD_TYPE Debug)

project("PhundrakSTL")

set(TGT "PhundrakSTL")
set(${TGT}_VERSION_MAJOR 0)
set(${TGT}_VERSION_MINOR 1)

set(CXX_COVERAGE_COMPILE_FLAGS "-pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion -Wsign-conversion -Wshadow -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Winline -Wunused -Wnon-virtual-dtor -std=c++17")
# set(CXX_COVERAGE_COMPILE_FLAGS "-Weverything")
set(CMAKE_CXX_FLAGS_DEBUG "${CXX_COVERAGE_COMPILE_FLAGS} -g3 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${CXX_COVERAGE_COMPILE_FLAGS} -O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "../debug/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_COVERAGE_COMPILE_FLAGS}")

include_directories(include)
file(GLOB SOURCES "src/*")
add_executable(${TGT} ${SOURCES})
