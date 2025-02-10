//
// Created by Joe (Personal) on 10/02/2025.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>
#include <event.h>
#include "amqp.h"

#define AMQP_HOST = "localhost" // TODO
#define AMQP_PORT 5672
#define AMQP_QUEUE "forest_queue"

static amqp_connection_state_t amqp_conn;
static amqp_socket_t *amqp_socket;

static void setup_ampq_connection()
{
    amqp_conn = amqp_new_connection();
    amqp_socket = amqp_tcp_socket_new(amqp_conn);
    if (!amqp_socket)
    {
        fprintf(stderr, "Failed to create AMPQ socket\n");
        return;
    }
    if (amqp_socket_open(amqp_socket, AMQP_HOST, AMQP_PORT))
    {

    }

}