//
// Created by Joe (Personal) on 10/02/2025.
//

#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "amqp.h"

#include <jansson.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include "config.h"
#include "queue.h"

#define FMQ_AMQP_PORT 5672

static struct evconnlistener *listener = NULL;


void start_amqp_listener(struct event_base *base, FMQ_Queue *queue)
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    sin.sin_port = htons(FMQ_AMQP_PORT);

    listener = evconnlistener_new_bind(
        base,
        NULL, // TODO callback
        NULL, // TODO context
       LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
       -1,
       (struct sockaddr *)&sin,
       sizeof(sin)
    );

    if (!listener)
    {
        perror("[AMPQ] Failed to bind to port 5672");
        exit(1);
    }
    FMQ_LOGGER(queue->log_level, "[FORESTMQ] Listening for AMQP connections on port %d\n", FMQ_AMQP_PORT);
}