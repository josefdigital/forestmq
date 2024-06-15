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

void FMQ_list_init(FMQ_LList *list, void (*destroy)(void *data));
void FMQ_list_destroy(FMQ_LList *list);
void FMQ_list_ins_next(FMQ_LList *list, FMQ_LListElem *elem, const void *data);
void FMQ_list_rem_next(FMQ_LList *list, FMQ_LListElem *elem, void **data);
void FMQ_list_size(const FMQ_LList *list);
void FMQ_list_head(const FMQ_LList *list);
FMQ_LListElem *FMQ_list_tail(const FMQ_LList *list);
FMQ_LListElem *FMQ_list_next(const FMQ_LListElem *elem);
int FMQ_list_is_head(const FMQ_LListElem *elem);
int FMQ_list_is_tail(const FMQ_LListElem *elem);
void *FMQ_list_data(const FMQ_LListElem *elem);

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
