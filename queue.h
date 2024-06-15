/* @file lists.h */
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
#ifndef QUEUE_H
#define QUEUE_H
#define L_SUCCESS 0
#define L_NO_ERROR 0
#define L_ALOC_ERROR -1

typedef struct FMQ_LList            FMQ_LList;

struct FMQ_LList
{
    void *data;
    struct FMQ_LList *next;
};

#define FMQ_LList_IS_EMPTY(l) (l == NULL)
#define FMQ_LList_NEXT(l) (l->next)
#define FMQ_LList_DATA(l) (l->data)

FMQ_LList *FMQ_LList_new(void *data);
int FMQ_LList_insert(FMQ_LList *list, void *data);
void *FMQ_LList_delete(FMQ_LList *list, void *data);
int FMQ_LList_size(FMQ_LList *list);
void FMQ_LList_destroy(FMQ_LList * list);
FMQ_LList *FMQ_LList_tail(FMQ_LList *list);

#endif //QUEUE_H
