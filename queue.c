/** @file LList.c */
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


FMQ_QNode *FMQ_QNode_new(void *data)
{
    FMQ_QNode *n = (FMQ_QNode*)malloc(sizeof(FMQ_QNode));
    if (n == NULL)
    {
        printf("Couldn't allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }
    n->data = data;
    n->next = NULL;
    return n; // TODO free
}

FMQ_Queue *FMQ_Queue_new(void)
{
    FMQ_Queue *q = (FMQ_Queue*)malloc(sizeof(FMQ_Queue));
    if (q == NULL)
    {
        printf("Couldn't allocate memory for new queue\n");
        exit(EXIT_FAILURE);
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q; // TODO free
}

void FMQ_Queue_enqueue(FMQ_Queue *queue, void *data)
{
    FMQ_QNode *tmpHeadNode = queue->head;
    FMQ_QNode *node = FMQ_QNode_new(data);
    if (queue->tail == NULL)
    {
        queue->head = node;
        queue->tail = node;
        return;
    }

    while (tmpHeadNode->next != NULL)
    {
        tmpHeadNode = tmpHeadNode->next;
    }
    tmpHeadNode->next = node;
}

FMQ_QNode *FMQ_Queue_dequeue(FMQ_Queue *queue)
{
    if (queue->head == NULL)
        return NULL;

    FMQ_QNode *node = queue->head;
    queue->head = queue->head->next;
    // if front becomes null reset queue
    if (queue->head == NULL)
        queue->tail = NULL;
    return node;
};