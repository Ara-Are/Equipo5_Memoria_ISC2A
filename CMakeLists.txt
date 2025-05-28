cmake_minimum_required(VERSION 3.29)
project(memoramaRaylib)

set(CMAKE_CXX_STANDARD 20)

set(RAYLIB_PATH "${CMAKE_SOURCE_DIR}/raylib-5.5_win64_mingw-w64")
include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

add_executable(memoramaRay src/memorama.cpp)

target_link_libraries(memoramaRay raylib)
if (WIN32)
    target_link_libraries(memoramaRay opengl32 gdi32 winmm)
endif()    