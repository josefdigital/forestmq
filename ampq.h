//
// Created by Joe (Personal) on 10/02/2025.
//

#ifndef AMPQ_H
#define AMPQ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <event.h>

#define AMPQ_HOST = "localhost" // TODO
#define AMPQ_PORT 5672

struct ampq_connection_state_t_ {
    amqp_connection_state_t conn;
    amqp_socket_t *socket;
    struct event *ev;
};




#endif //AMPQ_H
