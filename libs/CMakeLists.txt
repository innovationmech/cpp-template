# Internal project libraries

# Add core library if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/core/CMakeLists.txt)
    add_subdirectory(core)
else()
    message(WARNING "Core library CMakeLists.txt not found at ${CMAKE_CURRENT_SOURCE_DIR}/core/CMakeLists.txt")
endif()