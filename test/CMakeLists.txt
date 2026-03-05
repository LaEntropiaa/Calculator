find_package(cmocka REQUIRED)

add_executable(test_nodeArray test_ASTNodeArray.c)

target_link_libraries(test_nodeArray
    calculator_lib
    cmocka::cmocka
)

add_test(NAME nodeArray_tests COMMAND test_nodeArray)
