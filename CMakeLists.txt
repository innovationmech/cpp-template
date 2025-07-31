# Top-level CMakeLists.txt for cpp-template project
# Placeholder for main project configuration

cmake_minimum_required(VERSION 3.20)
project(cpp-template VERSION 1.0.0 DESCRIPTION "Modern C++ Project Template")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include subdirectories
add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(third_party)
add_subdirectory(tests)