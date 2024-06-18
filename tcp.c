/* @file tcp.c */
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
#include <stdio.h>
#include <stdlib.h>
#include <ulfius.h>
#include <jansson.h>
#include <string.h>
#include <stdbool.h>
#include "tcp.h"
#include "queue.h"
#include "config.h"
#define PORT 8005

static int callback_consumer(const struct _u_request *request,
    struct _u_response *response, void *queue)
{
    JSON_INDENT(4);
    const FMQ_QNode *node = FMQ_Queue_dequeue((FMQ_Queue*)queue);
    if (node == NULL)
    {
        FMQ_LOGGER("Queue is empty\n");
        ulfius_set_json_body_response(response, 204, json_pack("{s:s}", "message", NULL));
        return U_CALLBACK_CONTINUE;
    }
    const FMQ_Data *dataPtr = (FMQ_Data*)node->data;
    FMQ_LOGGER("Successfully dequeued message for consumer\n");
    ulfius_set_json_body_response(response, 200, json_pack("{s:s}", "message", dataPtr->message));
    free((FMQ_Data*)node->data);
    free((FMQ_QNode*)node);
    return U_CALLBACK_CONTINUE;
}

static int callback_provider(const struct _u_request *request,
    struct _u_response *response, void *queue)
{
    JSON_INDENT(4);
    json_t *json_body = ulfius_get_json_body_request(request, NULL);
    const char *message = json_string_value(json_object_get(json_body, "message"));
    bool destroy = json_boolean_value(json_object_get(json_body, "destroy"));
    if (destroy) {
        FMQ_QUEUE_destroy((FMQ_Queue*)queue);
        FMQ_LOGGER("Successfully destroyed queue\n");
        ulfius_set_json_body_response(response, 200, json_pack("{s:s}", "message", message));
        return U_CALLBACK_CONTINUE;
    }
    FMQ_LOGGER("Received: %s\n", message);
    FMQ_Data *data = (FMQ_Data*)malloc(sizeof(FMQ_Queue));
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    data->message = malloc(sizeof(char) * q->msg_size);
    strcpy(data->message, message);
    FMQ_Queue_enqueue((FMQ_Queue*)queue, data);

    ulfius_set_json_body_response(response, 200, json_pack("{s:s}", "message", message));

    json_decref(json_body);
    return U_CALLBACK_CONTINUE;
}

static int start_server(FMQ_TCP *tcp)
{
    struct _u_instance instance;

    if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK)
    {
        fprintf(stderr, "Error starting ulfius server\n");
        exit(EXIT_FAILURE);
    }
    // declare endpoints
    ulfius_add_endpoint_by_val(&instance, "POST", "/consumer", NULL, 0, &callback_consumer, tcp->queue);
    ulfius_add_endpoint_by_val(&instance, "POST", "/provider", NULL, 0, &callback_provider, tcp->queue);
    // start ulfius framework
    if (ulfius_start_framework(&instance) == U_OK)
    {
        printf("Starting server on http://localhost:%d\n", instance.port);
        getchar();
    }
    else
    {
        fprintf(stderr, "Error starting server...\n");
    }
    printf("Stopping server \n");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
    return 0;
}

FMQ_TCP *FMQ_TCP_new(FMQ_Queue *queue)
{
    FMQ_TCP *tcp = (FMQ_TCP*)malloc(sizeof(FMQ_TCP));
    tcp->queue = queue;
    tcp->start = start_server;
    return tcp;
}
