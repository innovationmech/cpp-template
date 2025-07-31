# Core library

# Create core library target
add_library(core
    src/core.cpp
)

# Set target properties
target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Set C++ standard for this target
target_compile_features(core PUBLIC cxx_std_17)

# Add alias for consistent naming
add_library(cpp-template::core ALIAS core)