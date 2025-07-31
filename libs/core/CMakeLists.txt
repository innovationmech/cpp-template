# Core library CMakeLists.txt

# Define the core library target
add_library(core STATIC)

# Set target properties
set_target_properties(core PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Add source files
target_sources(core
    PRIVATE
        src/core.cpp
        src/utils.cpp
    PUBLIC
        include/core/core.h
        include/core/utils.h
)

# Set up include directories
target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Apply compiler options from custom module
if(COMMAND apply_compiler_options)
    apply_compiler_options(core)
endif()

# Export the target for use by other parts of the project
set_target_properties(core PROPERTIES EXPORT_NAME Core)