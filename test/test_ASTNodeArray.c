#include "lexer.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_array_push(void **state) {
    (void) state;

    ASTNodeArray arr = ASTNodeArray_init(2);
    ASTNode node1 = {
        .type = NODE_NUMBER,
        .operator = { .number = 90 }
    };

    ASTNode node2 = {
        .type = NODE_NUMBER,
        .operator = { .number = 80 }
    };

    ASTNode node3 = {
        .type = NODE_NUMBER,
        .operator = { .number = 70 }
    };

    assert_int_equal(ASTNodeArray_push(&arr, node1), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 1);

    assert_int_equal(ASTNodeArray_push(&arr, node2), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 2);

    assert_int_equal(ASTNodeArray_push(&arr, node3), ARRAY_OK);
    assert_int_equal(ASTNodeArray_len(&arr), 3);
}



int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_array_push),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
