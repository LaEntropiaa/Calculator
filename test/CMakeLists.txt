find_package(cmocka REQUIRED)

add_executable(test_nodeArray test_ASTNodeArray.c)
add_executable(test_lexer test_lexer.c)
add_executable(test_parser test_parser.c)

target_link_libraries(test_nodeArray
    calculator_lib
    cmocka::cmocka
)

target_link_libraries(test_lexer
    calculator_lib
    cmocka::cmocka
)

target_link_libraries(test_parser
    calculator_lib
    cmocka::cmocka
)

add_test(NAME nodeArray_tests COMMAND test_nodeArray)
add_test(NAME lexer_tests COMMAND test_lexer)
add_test(NAME parser_tests COMMAND test_parser)
