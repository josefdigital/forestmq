#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include "../queue.h"

static void null_test_success(void **state)
{
    (void) state;
}

int main(void)
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(null_test_success)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
