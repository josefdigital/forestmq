/** @file queue.c */
/**
* MIT License
*
* Copyright (c) 2023 Joe Gasewicz
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"


FMQ_LList *FMQ_LList_new(void *data)
{
    FMQ_LList *l = malloc(sizeof(FMQ_LList));
    l->data = data;
    l->next = NULL;
    return l;
}

int FMQ_LList_insert(FMQ_LList *const list, void *data)
{
    FMQ_LList *tempList = list;
    while(tempList->next != NULL)
    {
        tempList = tempList->next;
    }
    tempList->next = malloc(sizeof(FMQ_LList));
    if (tempList->next == NULL)
    {
        printf("Error: unable to allocate memory");
        return L_ALOC_ERROR;
    }
    tempList->next->data = data;
    tempList->next->next = NULL;
    return L_NO_ERROR;
}

void *FMQ_LList_delete(FMQ_LList *list, void *data)
{
    FMQ_LList *tempPtr = list;
    FMQ_LList *prevPtr;
    // check if the current head of the list contains the same ref to data
    // & compare the address from void *data
    if (list->data == data)
    {
        tempPtr = list; // temporarily store node being deleted
        list = list->next;
        free(tempPtr->data);
        free(tempPtr);
    }
    else
    {
        while(tempPtr->data != data)
        {
            prevPtr = tempPtr;
            tempPtr = list->next;
        }
        prevPtr->next = tempPtr->next;
        free(tempPtr->data);
        free(tempPtr);
    }
    return data;
}

int FMQ_LList_size(FMQ_LList *list)
{
    int i = 0;
    FMQ_LList *currPtr = list;
    while (currPtr->next != NULL)
    {
        currPtr = currPtr->next;
        i++;
    }
    return i;
}

void FMQ_LList_destroy(FMQ_LList * list)
{
    FMQ_LList *tempPtr;
    if (list == NULL)
        return;
    while(tempPtr->next != NULL)
    {
        if (list->next == NULL)
        {
            free(list);
            break;
        }
        tempPtr = list;
        list = list->next;
        if (tempPtr)
            free(tempPtr);
    }
}

FMQ_LList *FMQ_LList_tail(FMQ_LList *list)
{
    FMQ_LList *tempPtr = list;
    if (tempPtr->next == NULL)
        return tempPtr;
    while (tempPtr->next != NULL)
        tempPtr = tempPtr->next;
    return tempPtr;
}