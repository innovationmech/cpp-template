# Main application

# Create main executable
add_executable(cpp-template-app
    main.cpp
)

# Link with internal libraries
target_link_libraries(cpp-template-app
    PRIVATE
        cpp-template::core
        cpp-template::example-module
)

# Include directories
target_include_directories(cpp-template-app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

# Set target properties
set_target_properties(cpp-template-app PROPERTIES
    OUTPUT_NAME "cpp-template"
)

# Add modules subdirectory
add_subdirectory(modules)