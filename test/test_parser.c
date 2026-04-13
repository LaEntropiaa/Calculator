#include "arena.h"
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

    ParseResult result = parse(&tokens);
    // Assert head is +
    assert_int_equal(result.tree->type, NODE_BINARY_OP);
    assert_int_equal(result.tree->data.binary.op, OP_ADD);

    assert_int_equal(result.tree->data.binary.left->type, NODE_INTEGER);
    assert_int_equal(result.tree->data.binary.left->data.integer, 2);


    assert_int_equal(
            result.tree->data.binary.right->type,
            NODE_BINARY_OP
            );
    assert_int_equal(
            result.tree->data.binary.right->data.binary.op, 
            OP_MUL
            );

    assert_int_equal(
            result.tree->data.binary.right->data.binary.right->type,
            NODE_INTEGER);
    assert_int_equal(
            result.tree->data.binary.right->data.binary.right->data.integer,
            789);

    assert_int_equal(
            result.tree->data.binary.right->data.binary.left->type, 
            NODE_BINARY_OP
            );
    assert_int_equal(
            result.tree->data.binary.right->data.binary.left->data.binary.op, 
            OP_DIV
            );

    assert_int_equal(
            result.tree->data.binary.right->data.binary.left->data.binary.right->type, 
            NODE_INTEGER
            );
    assert_int_equal(
            result.tree->data.binary.right->data.binary.left->data.binary.right->data.integer, 
            66
            );

    assert_int_equal(
            result.tree->data.binary.right->data.binary.left->data.binary.left->type, 
            NODE_INTEGER
            );
    assert_int_equal(
            result.tree->data.binary.right->data.binary.left->data.binary.left->data.integer, 
            3
            );
    arena_destroy(&result.arena);
}

int main(void) {
    const struct CMUnitTest tests [] = {
        cmocka_unit_test(test_parsing_basic_expression),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
