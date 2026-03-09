#include "lexer.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_string_to_number(void **state) {
    (void) state;

    char num[16] = "2333t55";
    size_t offset = 0;
    int64_t result = 0;
    assert_int_equal(string_to_number(num, &offset, &result), 0);

    assert_int_equal(offset, 4);
    assert_double_equal(result, 2333, 1e-6);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_string_to_number),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
