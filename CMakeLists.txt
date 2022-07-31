cmake_minimum_required(VERSION 3.20)
SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)
set( CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" )
set( CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )
set( CMAKE_MAKE_PROGRAM "C:/MinGW/bin/mingw32-make.exe")


# set the project name

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(Consoleoid)
add_executable(Consoleoid_exec Consoleoid.cpp)

