#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <check.h>
#include <string.h>
#include "../LList.h"


static void null_test_success(void **state)
{
    (void) state;
}

static void test_FMQ_LList_new1(void **state)
{
    char *expected = "Joe";
    typedef struct data_t {
        char *name;
        int age;
    } Data;

    Data *data;

    data = malloc(sizeof(Data));
    data->name = malloc(sizeof(expected));
    strcat(data->name, expected);
    data->age = 47;

    FMQ_LList *list = FMQ_LList_new(data);
    /* unit test code */
    Data *resultData = list->data;
    assert_string_equal(expected, resultData->name);
    assert_int_equal(47, resultData->age);
    /* clean up */
    free(data);
    free(list);
}

static void test_FMQ_LList_insert(void **state)
{
    // Setup
    typedef struct data_t {
        char *name;
    } Data;

    Data *data = malloc(sizeof(Data));
    Data *data3 = malloc(sizeof(Data)); // for negative test
    data->name = calloc(4, sizeof(char));
    strcpy(data->name, "Joe");
    FMQ_LList *list = FMQ_LList_new(data);

    // Setup for FMQ_LList_insert
    Data *data2 = malloc(sizeof(Data));
    data2->name = calloc(4, sizeof(char));
    strcpy(data2->name, "Ted");
    int result1 = FMQ_LList_insert(list, data2);

    // Test
    assert_int_equal(result1, L_NO_ERROR);
    assert_ptr_equal(list->data, data);
    assert_string_equal(((Data*)list->data)->name, "Joe");
    assert_ptr_equal(list->next->data, data2);
    assert_ptr_not_equal(list->next->data, data3);
    assert_string_equal(((Data*)list->next->data)->name, "Ted");
    free(list->next->data);
    free(list->next);
    free(list->data);
    free(list);
}

static void test_FMQ_LList_delete(void **state)
{
    // Setup
    typedef struct data_t {
        char *name;
    } Data;

    Data *data = malloc(sizeof(Data));
    data->name = calloc(4, sizeof(char));
    strcat(data->name, "Joe");
    FMQ_LList *list = FMQ_LList_new(data);

    // Insert another item
    Data *data2 = malloc(sizeof(Data));
    data2->name = calloc(4, sizeof(char));
    strcpy(data2->name, "Ted");
    FMQ_LList_insert(list, data2);

    // Insert another item
    Data *data3 = malloc(sizeof(Data));
    data3->name = calloc(4, sizeof(char));
    strcpy(data3->name, "Pat");
    FMQ_LList_insert(list, data3);

    // Delete the #2nd item
    void *result = FMQ_LList_delete(list, data2);

    // Test
    assert_ptr_equal(result, data2);
    assert_ptr_equal(list->data, data);
    assert_ptr_equal(list->next->data, data3);
    assert_ptr_equal(list->next->next, NULL);
    result = FMQ_LList_delete(list, NULL);

}

static void test_FMQ_LList_IS_EMPTY(void **state)
{
    int d = 1;
    FMQ_LList *l = FMQ_LList_new(&d);
    int result = FMQ_LList_IS_EMPTY(l);
    assert_int_equal(result, 0);
    result = FMQ_LList_IS_EMPTY(NULL);
    assert_int_equal(result, 1);
}

static void test_FMQ_LList_size(void **state)
{
    int d1 = 1;
    int d2 = 1;
    int d3 = 1;
    int d4 = 1;
    int result;
    FMQ_LList *l = FMQ_LList_new(&d1);
    FMQ_LList_insert(l, &d2);
    FMQ_LList_insert(l, &d3);
    FMQ_LList_insert(l, &d4);
    result = FMQ_LList_size(l);
    assert_int_equal(result, 3);
}

static void test_FMQ_LList_NEXT(void **state)
{
    int d1 = 1;
    int d2 = 2;
    int d3 = 3;
    int d4 = 4;
    FMQ_LList *l = FMQ_LList_new(&d1);
    FMQ_LList_insert(l, &d2);
    FMQ_LList_insert(l, &d3);
    FMQ_LList_insert(l, &d4);
    assert_ptr_equal(l->data, &d1);
    FMQ_LList *l2 = FMQ_LList_NEXT(l);
    assert_ptr_equal(l2->data, &d2);
    FMQ_LList *l3 = FMQ_LList_NEXT(l2);
    assert_ptr_equal(l3->data, &d3);
    FMQ_LList *l4 = FMQ_LList_NEXT(l3);
    assert_ptr_equal(l4->data, &d4);
}

static void test_FMQ_LList_DATA(void **state)
{
    int d1 = 1;
    FMQ_LList *l = FMQ_LList_new(&d1);
    int* result = FMQ_LList_DATA(l);
    assert_ptr_equal(result, &d1);
}

static void test_FMQ_LList_destroy(void **state)
{
    int d1 = 1;
    int d2 = 2;
    int d3 = 3;
    int d4 = 4;
    int result;
    FMQ_LList *l = FMQ_LList_new(&d1);
    FMQ_LList_insert(l, &d2);
    FMQ_LList_insert(l, &d3);
    FMQ_LList_insert(l, &d4);
    FMQ_LList_destroy(l);
    assert_int_equal(FMQ_LList_IS_EMPTY(l), 0);
}

static void test_FMQ_LList_tail(void **state)
{
    int d1 = 1;
    int d2 = 2;
    int d3 = 3;
    int d4 = 4;
    FMQ_LList *l = FMQ_LList_new(&d1);
    FMQ_LList_insert(l, &d2);
    FMQ_LList_insert(l, &d3);
    FMQ_LList_insert(l, &d4);
    FMQ_LList *l2 = FMQ_LList_NEXT(l);
    FMQ_LList *l3 = FMQ_LList_NEXT(l2);
    FMQ_LList *l4 = FMQ_LList_NEXT(l3);
    FMQ_LList *tail = FMQ_LList_tail(l);
    assert_ptr_equal(tail, l4);
}

static void TEST_FMQ_LList_enLList(void **state)
{
    (void) state;
    // Setup
    typedef struct data_t
    {
        char *name;
    } Data;

    Data *data = malloc(sizeof(Data));
    data->name = calloc(4, sizeof(char));
    strcpy(data->name, "Joe");
    FMQ_LList *LList = FMQ_LList_new(data);

    // Setup for FMQ_LList_insert
    Data *data2 = malloc(sizeof(Data));
    data2->name = calloc(4, sizeof(char));
    strcpy(data2->name, "Ted");
    int result1 = FMQ_LList_enLList(LList, data2);

    // Test
    Data *data3 = malloc(sizeof(Data)); // for negative test
    assert_int_equal(result1, L_NO_ERROR);
    assert_ptr_equal(LList->data, data);
    assert_string_equal(((Data*)LList->data)->name, "Joe");
    assert_ptr_equal(LList->next->data, data2);
    assert_ptr_not_equal(LList->next->data, data3);
    assert_string_equal(((Data*)LList->next->data)->name, "Ted");
    free(LList->next->data);
    free(LList->next);
    free(LList->data);
    free(LList);
}

static void TEST_FMQ_LList_deLList(void **state)
{
    (void) state;
    // Setup
    typedef struct data_t {
        char *name;
    } Data;

    Data *data = malloc(sizeof(Data));
    data->name = calloc(4, sizeof(char));
    strcat(data->name, "Joe");
    FMQ_LList *LList = FMQ_LList_new(data);

    // Insert another item
    Data *data2 = malloc(sizeof(Data));
    data2->name = calloc(4, sizeof(char));
    strcpy(data2->name, "Ted");
    FMQ_LList_enLList(LList, data2);

    // Insert another item
    Data *data3 = malloc(sizeof(Data));
    data3->name = calloc(4, sizeof(char));
    strcpy(data3->name, "Pat");
    FMQ_LList_enLList(LList, data3);

    // Delete the #2nd item
    void *LListHeadData = FMQ_LList_deLList(LList);
    // Test
    assert_ptr_equal(LListHeadData, data);

    // assert_ptr_equal(LList->data, data2);
    Data const *d = FMQ_LList_peak(LList);
    assert_string_equal(d->name, "Ted");
    assert_ptr_equal(LList->next->data, data3);
    assert_ptr_equal(LList->next->next, NULL);
}

static void TEST_FMQ_LList_peak(void **state)
{
    (void) state;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(test_FMQ_LList_new1),
        cmocka_unit_test(test_FMQ_LList_insert),
        cmocka_unit_test(test_FMQ_LList_delete),
        cmocka_unit_test(test_FMQ_LList_IS_EMPTY),
        cmocka_unit_test(test_FMQ_LList_size),
        cmocka_unit_test(test_FMQ_LList_NEXT),
        cmocka_unit_test(test_FMQ_LList_DATA),
        cmocka_unit_test(test_FMQ_LList_destroy),
        cmocka_unit_test(test_FMQ_LList_tail),
        cmocka_unit_test(TEST_FMQ_LList_enLList),
        cmocka_unit_test(TEST_FMQ_LList_deLList),
        cmocka_unit_test(TEST_FMQ_LList_peak)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
