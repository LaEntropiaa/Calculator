#include "lexer.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_tokenize_normal_expresion(void **state) {
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
}

static void test_tokenize_unrecognized_symbol(void **state) {
    (void) state;

    char expr[256] = " 2 j 3 / 66 } 789";
    ASTNodeArray tokens = {
        .len = 0,
        .cap = 0,
    };

    assert_int_equal(tokenize(expr, &tokens), LEXER_NOT_RECOGNIZED_SYMBOL);
    assert_int_equal(tokens.len, 0);
    assert_int_equal(tokens.cap, 0);
}

static void test_tokenize_wrong_sintax(void **state) {
    (void) state;

    char expr[256] = "2 3 / 66 789";
    ASTNodeArray tokens = {
        .len = 0,
        .cap = 0,
    };

    assert_int_equal(tokenize(expr, &tokens), LEXER_WRONG_SYNTAX);
    assert_int_equal(tokens.len, 0);
    assert_int_equal(tokens.cap, 0);
}

static void test_string_to_number_normal(void **state) {
    (void) state;

    char num[16] = "2333t55";
    size_t offset = 0;
    ASTNode result;

    assert_int_equal(tokenize_number(num, &offset, &result), LEXER_OK);

    assert_int_equal(offset, 4); // equal to t position in string
    assert_int_equal(result.type, NODE_INTEGER);
    assert_int_equal(result.data.integer, 2333);
}

static void test_string_to_number_overflow(void **state) {
    (void) state;

    // Number is INT64_MAX but with a extra 8 at the end
    char num[32] = "92233720368547758078yy7";
    size_t offset = 0;
    ASTNode result;
    assert_int_equal(tokenize_number(num, &offset, &result), LEXER_INT_OVERFLOW);
    // Technically it can trigger a buf overflow error but obvioulsy 
    // it will trigger int overflow error first
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_string_to_number_normal),
        cmocka_unit_test(test_string_to_number_overflow),
        cmocka_unit_test(test_tokenize_normal_expresion),
        cmocka_unit_test(test_tokenize_unrecognized_symbol),
        cmocka_unit_test(test_tokenize_wrong_sintax),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
