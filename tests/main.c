#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <check.h>
#include <string.h>
#include "../queue.h"


static void TEST_null_test_success(void **state)
{
    (void) state;
}

static void TEST_FMQ_QNode_new(void **state)
{
    (void) state;
    char *name = "Joe";
    FMQ_QNode *q = FMQ_QNode_new(name);
    assert_string_equal(q->data, "Joe");
    free(q);
}

static void TEST_FMQ_Queue_new(void **state)
{
    (void) state;
    FMQ_Queue *q = FMQ_Queue_new();
    assert_null(q->head);
    assert_null(q->tail);
    assert_int_equal(q->size, 0);
    free(q);
}


static void TEST_FMQ_Queue_enqueue(void **state)
{
    typedef struct Data {
       void *value;
    } Data;
    FMQ_Queue *q = FMQ_Queue_new();
    char *name1 = "Joe";
    char *name2 = "Terry";
    char *name3 = "Tarquin";
    char *name4 = "Nigel";
    char *name5 = "Frank";

    Data *d1 = malloc(sizeof(Data));
    d1->value = calloc(4, sizeof(char));
    strcat(d1->value, name1);
    FMQ_QNode *n1 = FMQ_QNode_new(d1);

    Data *d2 = malloc(sizeof(Data));
    d2->value = calloc(6, sizeof(char));
    strcat(d2->value, name2);
    FMQ_QNode *n2 = FMQ_QNode_new(d2);

    Data *d3 = malloc(sizeof(Data));
    d3->value = calloc(6, sizeof(char));
    strcat(d3->value, name3);
    FMQ_QNode *n3 = FMQ_QNode_new(d3);

    Data *d4 = malloc(sizeof(Data));
    d4->value = calloc(6, sizeof(char));
    strcat(d4->value, name4);
    FMQ_QNode *n4 = FMQ_QNode_new(d4);

    Data *d5 = malloc(sizeof(Data));
    d5->value = calloc(6, sizeof(char));
    strcat(d5->value, name5);
    FMQ_QNode *n5 = FMQ_QNode_new(d5);

    FMQ_Queue_enqueue(q, n1);
    FMQ_Queue_enqueue(q, n2);
    FMQ_Queue_enqueue(q, n3);
    FMQ_Queue_enqueue(q, n4);
    FMQ_Queue_enqueue(q, n5);

    const Data* resultData = q->head->data;
    const Data* resultData2 = q->head->next->data;
    const Data* resultData3 = q->head->next->next->data;
    const Data* resultData4 = q->head->next->next->next->data;
    const Data* resultData5 = q->head->next->next->next->next->data;

    assert_string_equal(resultData->value, d1);
    assert_string_equal(resultData2->value, d2);
    assert_string_equal(resultData3->value, d3);
    assert_string_equal(resultData4->value, d4);
    assert_string_equal(resultData5->value, d5);

    free(d1->value);
    free(d1);
    free(d2->value);
    free(d2);
    free(d3->value);
    free(d3);
    free(d4->value);
    free(d4);
    free(d5->value);
    free(d5);
    free(n1);
    free(n2);
    free(n3);
    free(n4);
    free(n5);
    free(q);
}

static void TEST_FMQ_Queue_dequeue(void **state)
{
    typedef struct Data {
        void *value;
    } Data;
    FMQ_Queue *q = FMQ_Queue_new();
    char *name1 = "Joe";
    char *name2 = "Terry";
    char *name3 = "Tarquin";
    char *name4 = "Nigel";
    char *name5 = "Frank";

    Data *d1 = malloc(sizeof(Data));
    d1->value = calloc(4, sizeof(char));
    strcat(d1->value, name1);
    FMQ_QNode *n1 = FMQ_QNode_new(d1);

    Data *d2 = malloc(sizeof(Data));
    d2->value = calloc(6, sizeof(char));
    strcat(d2->value, name2);
    FMQ_QNode *n2 = FMQ_QNode_new(d2);

    Data *d3 = malloc(sizeof(Data));
    d3->value = calloc(8, sizeof(char));
    strcat(d3->value, name3);
    FMQ_QNode *n3 = FMQ_QNode_new(d3);

    Data *d4 = malloc(sizeof(Data));
    d4->value = calloc(6, sizeof(char));
    strcat(d4->value, name4);
    FMQ_QNode *n4 = FMQ_QNode_new(d4);

    Data *d5 = malloc(sizeof(Data));
    d5->value = calloc(6, sizeof(char));
    strcat(d5->value, name5);
    FMQ_QNode *n5 = FMQ_QNode_new(d5);

    FMQ_Queue_enqueue(q, n1);
    FMQ_Queue_enqueue(q, n2);
    FMQ_Queue_enqueue(q, n3);
    FMQ_Queue_enqueue(q, n4);
    FMQ_Queue_enqueue(q, n5);

    FMQ_Queue_dequeue(q);
    Data* resultData = q->head->data;
    assert_string_equal(resultData->value, d2);

    FMQ_Queue_dequeue(q);
    resultData = q->head->data;
    assert_string_equal(resultData->value, d3);

    FMQ_Queue_dequeue(q);
    resultData = q->head->data;
    assert_string_equal(resultData->value, d4);

    FMQ_Queue_dequeue(q);
    resultData = q->head->data;
    assert_string_equal(resultData->value, d5);

    free(d1->value);
    free(d1);
    free(d2->value);
    free(d2);
    free(d3->value);
    free(d3);
    free(d4->value);
    free(d4);
    free(d5->value);
    free(d5);
    free(n1);
    free(n2);
    free(n3);
    free(n4);
    free(n5);
    free(q);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(TEST_null_test_success),
        cmocka_unit_test(TEST_FMQ_QNode_new),
        cmocka_unit_test(TEST_FMQ_Queue_new),
        cmocka_unit_test(TEST_FMQ_Queue_enqueue),
        cmocka_unit_test(TEST_FMQ_Queue_dequeue)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
