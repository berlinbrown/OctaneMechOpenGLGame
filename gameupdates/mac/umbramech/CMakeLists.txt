cmake_minimum_required(VERSION 3.10)
project(glants VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/network/*.cpp
)

# Header files (optional to list)
include_directories(
    include
    include/network
)

# Platform-specific settings
if(WIN32)
    set(PLATFORM_LIBS opengl32)
else()
    set(PLATFORM_LIBS GL GLU X11 Xi Xmu Xxf86vm pthread m)
endif()

# External dependencies (GLFW, GLEW, etc.)
# You can use find_package or link libraries manually

# Example with GLFW and GLEW
find_package(OpenGL REQUIRED)

# Add executable
add_executable(glants ${SRC_FILES})

# Link libraries
target_link_libraries(glants
    ${OPENGL_LIBRARIES}
    ${PLATFORM_LIBS}
)
