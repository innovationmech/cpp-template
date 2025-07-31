cmake_minimum_required(VERSION 3.20)
project(cpp-template VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Include compiler options if file exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerOptions.cmake)
    include(CompilerOptions)
endif()

# Add subdirectories only if they contain CMakeLists.txt
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/libs/CMakeLists.txt)
    add_subdirectory(libs)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)
    add_subdirectory(src)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/CMakeLists.txt)
    add_subdirectory(third_party)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt)
    add_subdirectory(examples)
endif()

# Enable testing
enable_testing()