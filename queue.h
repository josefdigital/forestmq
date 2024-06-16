/* @file queue.h */
/**
* MIT License
*
* Copyright (c) 2024 Joe Gasewicz
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
#ifndef QUEUE_H
#define QUEUE_H

typedef struct FMQ_QNode    FMQ_QNode;
typedef struct FMQ_Queue    FMQ_Queue;
typedef struct FMQ_Data     FMQ_Data;

struct FMQ_QNode
{
    void                    *data;
    FMQ_QNode               *next;
};

struct FMQ_Queue
{
    int                     size;
    FMQ_QNode               *head;
    FMQ_QNode               *tail;
};

struct FMQ_Data
{
    void                    *message;
};

FMQ_QNode *FMQ_QNode_new(void *data);
FMQ_Queue *FMQ_Queue_new(void);
void FMQ_Queue_enqueue(FMQ_Queue *queue, void *data);
FMQ_QNode *FMQ_Queue_dequeue(FMQ_Queue *queue);

#endif //QUEUE_H
