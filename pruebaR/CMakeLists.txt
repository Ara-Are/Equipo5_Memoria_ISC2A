cmake_minimum_required(VERSION 3.30)
project(pruebaR)

set(CMAKE_CXX_STANDARD 20)

set(RAYLIB_PATH "${CMAKE_SOURCE_DIR}/raylib-5.5_win64_mingw-w64")
include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

add_executable(pruebaR main.cpp)
add_executable(img img.cpp)
# Enlazar Raylib
# Dependencias adicionales en Windows
foreach(target IN ITEMS pruebaR img)
    target_link_libraries(${target}
            raylib           # Librería principal
            opengl32         # Dependencia OpenGL
            gdi32            # Sistema gráfico de Windows
            winmm            # Multimedia de Windows
    )
endforeach()

