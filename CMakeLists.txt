cmake_minimum_required(VERSION 3.20)
project(cpp-template VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Include compiler options
include(CompilerOptions)

# Add subdirectories
add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(third_party)
add_subdirectory(tests)

# Enable testing
enable_testing()