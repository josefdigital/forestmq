#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <check.h>
#include <string.h>
#include "../queue.h"
#include "queue_tests.h"


static void TEST_null_test_success(void **state)
{
    (void) state;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(TEST_null_test_success),
        cmocka_unit_test(TEST_FMQ_QNode_new),
        cmocka_unit_test(TEST_FMQ_Queue_new),
        cmocka_unit_test(TEST_FMQ_Queue_enqueue),
        cmocka_unit_test(TEST_FMQ_Queue_dequeue),
        cmocka_unit_test(TEST_FMQ_QUEUE_PEAK),
        cmocka_unit_test(TEST_FMQ_QUEUE_SIZE),
        cmocka_unit_test(TEST_FMQ_QUEUE_detroy)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
