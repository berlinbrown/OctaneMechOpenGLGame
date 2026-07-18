cmake_minimum_required(VERSION 3.10)
project(mechframework VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
    include
)

add_compile_options(-Wno-deprecated-declarations)

set(SRC_FILES
    src/main.cpp
    src/bot.cpp
    src/camera.cpp
    src/globals.cpp
    src/lights.cpp
    src/objects.cpp
    src/renderants.cpp
    src/grid.cpp
    src/cube.cpp
    src/norm_cube.cpp
    src/static.cpp
    src/pyramid.cpp
    src/stars.cpp
    src/world.cpp
    src/wirebox.cpp
    src/plist.cpp
    src/garden.cpp
    src/legacy_stubs.cpp
)

add_executable(mechframework ${SRC_FILES})

target_precompile_headers(mechframework PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/precompiled.hpp)

target_link_libraries(mechframework
    "-framework OpenGL"
    "-framework GLUT"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)
