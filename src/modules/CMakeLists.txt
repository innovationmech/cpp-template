# Application modules

# Example module library
add_library(example-module
    example/example_module.cpp
)

# Set target properties
target_include_directories(example-module
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Link with core library
target_link_libraries(example-module
    PUBLIC
        cpp-template::core
)

# Set C++ standard
target_compile_features(example-module PUBLIC cxx_std_17)

# Add alias
add_library(cpp-template::example-module ALIAS example-module)