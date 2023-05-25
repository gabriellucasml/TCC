cmake_minimum_required(VERSION 3.24)
project(untitled)

set(CMAKE_CXX_STANDARD 23)

add_executable(untitled main.cpp include/well.h include/field.h src/well.cpp src/field.cpp include/solver.h src/solver.cpp)
