# Unit tests

if(GTest_FOUND)
    # Core library tests
    add_executable(test_core
        test_core.cpp
    )
    
    target_link_libraries(test_core
        PRIVATE
            cpp-template::core
            GTest::gtest
            GTest::gtest_main
    )
    
    target_include_directories(test_core
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )
    
    # Add test to CTest
    add_test(NAME CoreTests COMMAND test_core)
    
    # Module tests
    add_executable(test_modules
        test_modules.cpp
    )
    
    target_link_libraries(test_modules
        PRIVATE
            cpp-template::example-module
            GTest::gtest
            GTest::gtest_main
    )
    
    target_include_directories(test_modules
        PRIVATE
            ${PROJECT_SOURCE_DIR}/src/modules
    )
    
    # Add test to CTest
    add_test(NAME ModuleTests COMMAND test_modules)
endif()