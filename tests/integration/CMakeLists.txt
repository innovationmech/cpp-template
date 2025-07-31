# Integration tests

if(GTest_FOUND)
    # Application integration tests
    add_executable(test_integration
        test_application_integration.cpp
        test_module_interactions.cpp
    )
    
    target_link_libraries(test_integration
        PRIVATE
            cpp-template::core
            cpp-template::example-module
            GTest::gtest
            GTest::gtest_main
    )
    
    target_include_directories(test_integration
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src/modules
    )
    
    # Add test to CTest
    add_test(NAME IntegrationTests COMMAND test_integration)
    
    # Set test properties
    set_tests_properties(IntegrationTests PROPERTIES
        TIMEOUT 30
        LABELS "integration"
    )
endif()