# Source directory CMakeLists.txt
# Build configuration for application source code

# Create a library for the public interface implementation
add_library(cpp-template-impl STATIC
    core_impl.cpp
    utils_impl.cpp
)

# Set target properties
set_target_properties(cpp-template-impl PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Link to the internal core library
target_link_libraries(cpp-template-impl
    PUBLIC
        core
)

# Set up include directories
target_include_directories(cpp-template-impl
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/core/include
)

# Apply compiler options from custom module
if(COMMAND apply_compiler_options)
    apply_compiler_options(cpp-template-impl)
endif()

# Add the modules subdirectory
add_subdirectory(modules)

# Create the main executable (placeholder for now)
add_executable(cpp-template-app
    main.cpp
)

# Set target properties for the executable
set_target_properties(cpp-template-app PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Link the executable to the implementation library and modules
target_link_libraries(cpp-template-app
    PRIVATE
        cpp-template-impl
        data-processor
        config-manager
)

# Apply compiler options to the executable
if(COMMAND apply_compiler_options)
    apply_compiler_options(cpp-template-app)
endif()