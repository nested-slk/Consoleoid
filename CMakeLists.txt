cmake_minimum_required(VERSION 3.20)
SET (CMAKE_C_COMPILER_WORKS 1)
SET (CMAKE_CXX_COMPILER_WORKS 1)
# set( CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" )
# set( CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )
# set( CMAKE_MAKE_PROGRAM "C:/MinGW/bin/mingw32-make.exe")
#-lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-Wl,--no-undefined -static-libgcc -Wl,-O1 -Wl,--as-needed -Wl,--sort-common -s")
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(Consoleoid LANGUAGES CXX)
add_executable(Consoleoid_exec Consoleoid.cpp)

