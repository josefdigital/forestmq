//
// Created by Joe Gasewicz on 15/06/2024.
//

#ifndef TCP_H
#define TCP_H
typedef struct FMQ_TCP      FMQ_TCP;

struct FMQ_TCP
{
    int     (*start)(void);
};

FMQ_TCP *FMQ_TCP_new();

#endif //TCP_H
