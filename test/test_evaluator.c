#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

static void test_basic_evaluation(void** state) {
    (void) state;

    char expr[256] = "2 + 4 * 40 / 2";
    ASTNodeArray context;

    tokenize(expr, &context);
    AST tree = parse(&context);
    uint64_t value = evaluate(tree.head);

    assert_int_equal(value, 82);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_basic_evaluation),
    };

    cmocka_run_group_tests(tests, NULL, NULL);
    return EXIT_SUCCESS;
}
