#include "lexer.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_array_push(void **state) {
    (void) state;

    // We use 2 to force resize and checking anything wrong with malloc
    ASTNodeArray arr = ASTNodeArray_init(2);
    ASTNode node1 = {
        .type = NODE_NUMBER,
        .data = { .number = 90 }
    };

    ASTNode node2 = {
        .type = NODE_NUMBER,
        .data = { .number = 80 }
    };

    ASTNode node3 = {
        .type = NODE_NUMBER,
        .data = { .number = 70 }
    };

    assert_int_equal(ASTNodeArray_push(&arr, node1), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 1);

    assert_int_equal(ASTNodeArray_push(&arr, node2), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 2);

    assert_int_equal(ASTNodeArray_push(&arr, node3), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 3);

    ASTNodeArray_free(&arr);
}

static void test_array_pop(void **state) {
    (void) state;

    // Set to force desize
    ASTNodeArray arr = ASTNodeArray_init(16);
    ASTNode node1 = {
        .type = NODE_NUMBER,
        .data = { .number = 90 }
    };

    ASTNode node2 = {
        .type = NODE_NUMBER,
        .data = { .number = 80 }
    };

    ASTNode node3 = {
        .type = NODE_NUMBER,
        .data = { .number = 70 }
    };

    assert_int_equal(ASTNodeArray_push(&arr, node1), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 1);

    assert_int_equal(ASTNodeArray_push(&arr, node2), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 2);

    assert_int_equal(ASTNodeArray_push(&arr, node3), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 3);

    ASTNode node4;
    assert_int_equal(ASTNodeArray_pop(&arr, 1, &node4), ARRAY_OK);
    assert_int_equal(node4.type, NODE_NUMBER);
    assert_int_equal(node4.data.number, 80);

    ASTNodeArray_free(&arr);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_array_push),
        cmocka_unit_test(test_array_pop),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
