cmake_minimum_required(VERSION 3.10)
project(basictut VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

# Header files (optional to list)
include_directories(
    include
)

# Platform-specific settings
set(PLATFORM_LIBS pthread m)

# Add executable
add_executable(basictut ${SRC_FILES})

# Link libraries
target_link_libraries(basictut
    ${PLATFORM_LIBS}
)
