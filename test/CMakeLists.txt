find_package(cmocka REQUIRED)

add_executable(test_parser test_parser.c)

target_link_libraries(test_parser
    calculator_lib
    cmocka::cmocka
)

add_test(NAME parser_tests COMMAND test_parser)
