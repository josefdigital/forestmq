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

typedef struct FMQ_LList        FMQ_LList;
typedef struct FMQ_LListElem    FMQ_LListElem;
typedef struct FMQ_Queue        FMQ_Queue;

FMQ_LList *FMQ_LList_new(void (*destroy)(void *data));
void FMQ_list_destroy(FMQ_LList *list);
void FMQ_list_ins_next(FMQ_LList *list, FMQ_LListElem *elem, const void *data);
void FMQ_list_rem_next(FMQ_LList *list, FMQ_LListElem *elem, void **data);

#define FMQ_GET_LIST_SIZE (list) ((list)->size)
#define FMQ_GET_LIST_HEAD (list) ((list)->head)
#define FMQ_GET_LIST_TAIL (list) ((list)->tail)
#define FMQ_GET_IS_LIST_HEAD (list, elem) ((elem) == (list)->head ? 1 : 0)
#define FMQ_IS_LIST_TAIL ((elem)->next == NULL ? 1 : 0)
#define FMQ_GET_LIST_DATA(elem) ((elem)->data)
#define FMQ_GET_LIST_NEXT(elem) ((elem)->next)

struct FMQ_LListElem
{
    void                        *data;
    struct FMQ_LListE           *next;
};

struct FMQ_LList
{
    int                         size;
    int                         (*match)(const void *key1, const void *key2);
    void                        (*destroy)(void *data);
    FMQ_LListElem               *head;
    FMQ_LListElem               *tail;
};

struct FMQ_Queue
{

};

#endif //QUEUE_H
