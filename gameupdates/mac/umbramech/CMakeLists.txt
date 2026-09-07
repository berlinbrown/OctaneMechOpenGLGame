cmake_minimum_required(VERSION 3.16)
project(umbramech VERSION 1.0 LANGUAGES CXX)

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

# Exclude legacy/unused source that doesn't compile in current build.
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/DriverBots.cpp")

# Header files (optional to list)
include_directories(
    include
    include/network
)

# Platform-specific settings
#if(WIN32)
#    set(PLATFORM_LIBS opengl32)
#else()
#    set(PLATFORM_LIBS GL GLU X11 Xi Xmu Xxf86vm pthread m)
#endif()

add_compile_options(-Wno-deprecated-declarations)

# Add executable
add_executable(umbramech ${SRC_FILES})

target_precompile_headers(umbramech PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/precompiled.hpp)

# Link libraries
target_link_libraries(umbramech
    "-framework OpenGL"
    "-framework GLUT" 
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)

# Native asynchronous playback; assets travel with the executable.
enable_language(OBJCXX)
set_property(TARGET umbramech PROPERTY OBJCXX_STANDARD 17)
set_property(TARGET umbramech PROPERTY OBJCXX_STANDARD_REQUIRED ON)
target_sources(umbramech PRIVATE src/audio.mm)
set_source_files_properties(src/audio.mm PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_FLAGS "-fobjc-arc")
target_link_libraries(umbramech "-framework AVFoundation")
target_compile_definitions(umbramech PRIVATE UMBRA_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources")
add_custom_command(TARGET umbramech POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:umbramech>/resources")

include(CTest)
if(BUILD_TESTING)
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME input_regression COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_input.py)
        add_test(NAME core_regression COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_core.py)
    endif()
endif()
