add_executable(run_project_tests tests.cpp)

target_compile_definitions(run_project_tests PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

target_include_directories(run_project_tests PRIVATE ../src)

target_link_libraries(run_project_tests PRIVATE miemsense_logic)

add_test(NAME AllTests COMMAND run_project_tests)
