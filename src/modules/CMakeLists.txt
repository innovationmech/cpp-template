# Modules CMakeLists.txt Build configuration for application modules

# Create a library for the data processing module
add_library(data-processor STATIC data_processor.cpp)

# Create a library for the configuration module
add_library(config-manager STATIC config_manager.cpp)

# Set target properties for data-processor
set_target_properties(
    data-processor
    PROPERTIES CXX_STANDARD 17
               CXX_STANDARD_REQUIRED ON
               CXX_EXTENSIONS OFF)

# Set target properties for config-manager
set_target_properties(
    config-manager
    PROPERTIES CXX_STANDARD 17
               CXX_STANDARD_REQUIRED ON
               CXX_EXTENSIONS OFF)

# Link data-processor to core library and config-manager (showing inter-module dependency)
target_link_libraries(data-processor PUBLIC core config-manager)

# Link config-manager to core library
target_link_libraries(config-manager PUBLIC core)

# Set up include directories for data-processor
target_include_directories(
    data-processor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/core/include)

# Set up include directories for config-manager
target_include_directories(
    config-manager
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/core/include)

# Apply compiler options from custom module
if(COMMAND apply_compiler_options)
    apply_compiler_options(data-processor)
    apply_compiler_options(config-manager)
endif()
