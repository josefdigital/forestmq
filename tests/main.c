#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdbool.h>
#include "../queue.h"

static void TEST_null_test_success(void **state)
{
    (void) state;
}

static void TEST_FMQ_LList_new(void **state)
{
    (void) state;
    FMQ_LList *l = FMQ_LList_new(NULL);
    assert_int_equal(l->size, 0);
    assert_null(l->head);
    assert_null(l->tail);
    assert_null(l->destroy);
    free(l);
}

int main(void)
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(TEST_null_test_success),
        cmocka_unit_test(TEST_FMQ_LList_new)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
