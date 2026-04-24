#include "arraylist.h"
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
    ASTNode node;
    TokenizeResult tokens = tokenize(expr);

    assert_true(tokens.is_valid);
    assert_int_equal(arraylist_size(tokens.arr), 7);

    arraylist_get(tokens.arr, 0, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 2);

    arraylist_get(tokens.arr, 0, &node);
    assert_int_equal(node.type, NODE_BINARY_OP);
    assert_int_equal(node.data.binary.op, OP_ADD);

    arraylist_get(tokens.arr, 0, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 3);

    arraylist_get(tokens.arr, 0, &node);
    assert_int_equal(node.type, NODE_BINARY_OP);
    assert_int_equal(node.data.binary.op, OP_DIV);

    arraylist_get(tokens.arr, 0, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 66);

    arraylist_get(tokens.arr, 0, &node);
    assert_int_equal(node.type, NODE_BINARY_OP);
    assert_int_equal(node.data.binary.op, OP_MUL);

    arraylist_get(tokens.arr, 0, &node);
    assert_int_equal(node.type, NODE_INTEGER);
    assert_int_equal(node.data.integer, 789);
}

static void test_tokenize_unrecognized_symbol(void **state) {
    (void) state;

    char expr[256] = " 2 j 3 / 66 } 789";
    TokenizeResult tokens = tokenize(expr);

    assert_false(tokens.is_valid);
    assert_uint_equal(tokens.err, LEXER_WRONG_SYNTAX);
}

static void test_tokenize_wrong_sintax(void **state) {
    (void) state;

    char expr[256] = "2 3 / 66 789";
    TokenizeResult tokens = tokenize(expr);

    assert_false(tokens.is_valid);
    assert_uint_equal(tokens.err, LEXER_WRONG_SYNTAX);
}

static void test_string_to_number_normal(void **state) {
    (void) state;

    char num[16] = "2333t55";
    size_t offset = 0;
    ASTNodeResult result = tokenize_number(num, &offset);

    assert_true(result.is_valid);

    assert_int_equal(offset, 4); // equal to t position in string
    assert_int_equal(result.node.type, NODE_INTEGER);
    assert_int_equal(result.node.data.integer, 2333);
}

static void test_string_to_number_overflow(void **state) {
    (void) state;

    // Number is INT64_MAX but with a extra 8 at the end
    char num[32] = "92233720368547758078yy7";
    size_t offset = 0;
    ASTNodeResult result = tokenize_number(num, &offset);
    assert_false(result.is_valid);
    assert_uint_equal(result.err, LEXER_INT_OVERFLOW);
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
