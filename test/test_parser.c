#include "lexer.h"
#include "parser.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_parsing_basic_expression(void **state) {
    (void) state;

    char expr[256] = "2 + 3 / 66 * 789";
    ASTNodeArray tokens;
    ASTNode node;
    
    assert_int_equal(tokenize(expr, &tokens), LEXER_OK);
    assert_int_equal(tokens.len, 7);

    ASTNodeArray_get(&tokens, 0, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 2);

    ASTNodeArray_get(&tokens, 1, &node);
    assert_int_equal(node.type, NODE_BINARY_OP);
    assert_int_equal(node.data.binary.op, OP_ADD);

    ASTNodeArray_get(&tokens, 2, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 3);

    ASTNodeArray_get(&tokens, 3, &node);
    assert_int_equal(node.type, NODE_BINARY_OP);
    assert_int_equal(node.data.binary.op, OP_DIV);

    ASTNodeArray_get(&tokens, 4, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 66);

    ASTNodeArray_get(&tokens, 5, &node);
    assert_int_equal(node.type, NODE_BINARY_OP);
    assert_int_equal(node.data.binary.op, OP_MUL);

    ASTNodeArray_get(&tokens, 6, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 789);

    AST tree = parse(&tokens);
    // Assert head is +
    assert_int_equal(tree.head->type, NODE_BINARY_OP);
    assert_int_equal(tree.head->data.binary.op, OP_ADD);

    assert_int_equal(tree.head->data.binary.left->type, NODE_INTEGER);
    assert_int_equal(tree.head->data.binary.left->data.integer, 2);


    assert_int_equal(
            tree.head->data.binary.right->type,
            NODE_BINARY_OP
            );
    assert_int_equal(
            tree.head->data.binary.right->data.binary.op, 
            OP_MUL
            );

    assert_int_equal(
            tree.head->data.binary.right->data.binary.right->type,
            NODE_INTEGER);
    assert_int_equal(
            tree.head->data.binary.right->data.binary.right->data.integer,
            789);

    assert_int_equal(
            tree.head->data.binary.right->data.binary.left->type, 
            NODE_BINARY_OP
            );
    assert_int_equal(
            tree.head->data.binary.right->data.binary.left->data.binary.op, 
            OP_DIV
            );

    assert_int_equal(
            tree.head->data.binary.right->data.binary.left->data.binary.right->type, 
            NODE_INTEGER
            );
    assert_int_equal(
            tree.head->data.binary.right->data.binary.left->data.binary.right->data.integer, 
            66
            );

    assert_int_equal(
            tree.head->data.binary.right->data.binary.left->data.binary.left->type, 
            NODE_INTEGER
            );
    assert_int_equal(
            tree.head->data.binary.right->data.binary.left->data.binary.left->data.integer, 
            3
            );
}

int main(void) {
    const struct CMUnitTest tests [] = {
        cmocka_unit_test(test_parsing_basic_expression),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
