#include <stdio.h>
#include <stdlib.h>
#include "tcp.h"

int main(void)
{
    FMQ_TCP *tcp = FMQ_TCP_new();
    const int err = tcp->start();
    if (tcp != NULL)
    {
        free(tcp);
    }
    if (err)
    {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
