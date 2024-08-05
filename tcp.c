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
#include <time.h>
#include "tcp.h"
#include "queue.h"
#include "config.h"


static int callback_consumer(const struct _u_request *request,
    struct _u_response *response, void *queue)
{
    JSON_INDENT(4);
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    const FMQ_QNode *node = FMQ_Queue_dequeue((FMQ_Queue*)queue);
    if (node == NULL)
    {
        FMQ_LOGGER(q->log_level ,"{consumer}: Queue is empty\n");
        json_t *root = json_object();
        json_object_set_new(root, "error", json_string("Queue is empty!"));
        ulfius_set_json_body_response(response, 200, json_pack("o*", root));
        json_decref(root);
        return U_CALLBACK_CONTINUE;
    }
    const FMQ_Data *dataPtr = (FMQ_Data*)node->data;
    json_t *message_load = json_loads(dataPtr->message, JSON_ENCODE_ANY, NULL);
    if (json_is_null(message_load))
    {
        char err_msg[] = "{consumer}: Error: No message in stored queue node.\n";
        FMQ_LOGGER(q->log_level, "{consumer}: Error: No message in stored queue node.\n");
        ulfius_set_json_body_response(response, 500, json_pack("{s:s}", "error", err_msg));
        return U_CALLBACK_CONTINUE;
    }
    char *msg_dump = json_dumps(message_load, JSON_COMPACT);
    FMQ_LOGGER(q->log_level, "{consumer}: Successfully dequeued message for consumer\n");
    FMQ_LOGGER(q->log_level, "{consumer}: Received: %s\n", msg_dump);
    free(msg_dump);
    free((FMQ_Data*)node->data);
    free((FMQ_QNode*)node);
    json_t *root = json_object();
    JSON_INDENT(4);
    json_object_set_new(root, "message", message_load);

    ulfius_set_json_body_response(response, 200, json_pack("o*", root));
    json_decref(root);
    return U_CALLBACK_CONTINUE;
}

static int callback_provider(const struct _u_request *request,
    struct _u_response *response, void *queue)
{
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    JSON_INDENT(4);
    json_t *json_body = ulfius_get_json_body_request(request, NULL);
    json_t *message = json_object_get(json_body, "message");
    bool destroy = json_boolean_value(json_object_get(json_body, "destroy"));
    if (destroy) {
        FMQ_QUEUE_destroy((FMQ_Queue*)queue);
        FMQ_LOGGER(q->log_level, "{provider}: Successfully destroyed queue\n");
        ulfius_set_json_body_response(response, 200, json_pack("{s:s}", "message", message));
        return U_CALLBACK_CONTINUE;
    }
    if (message == NULL)
    {
        json_t *root = json_object();
        char err_msg[] = "Provider did not include a message property in the request body";
        FMQ_LOGGER(q->log_level, "{provider}: Error: %s\n", err_msg);
        json_object_set_new(root, "error", json_string(err_msg));
        ulfius_set_json_body_response(response, 500, json_pack("o*", root));
        json_decref(root);
        return U_CALLBACK_CONTINUE;
    }
    char *message_dump = json_dumps(message, JSON_COMPACT);
    FMQ_LOGGER(q->log_level, "{provider}: Received: %s\n", message_dump);
    free(message_dump);
    FMQ_Data *data = (FMQ_Data*)malloc(sizeof(FMQ_Queue));
    data->message = malloc(sizeof(char) * q->msg_size);
    strcpy(data->message, json_dumps(message, JSON_COMPACT));
    FMQ_Queue_enqueue((FMQ_Queue*)queue, data);

    ulfius_set_json_body_response(response, 200, json_pack("{s:o*}", "message", message));

    json_decref(json_body);
    return U_CALLBACK_CONTINUE;
}

static int callback_health(const struct _u_request *request,
    struct _u_response *response, void *queue)
{

    time_t rawtime_start, rawtime_end;
    struct tm *req_start;
    struct tm *req_end;

    time(&rawtime_start);
    req_start = localtime(&rawtime_start);

    json_t *root = json_object();
    bool queue_is_empty = false;
    JSON_INDENT(4);
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    const FMQ_QNode *node = FMQ_QUEUE_PEEK((FMQ_Queue*)queue);
    const int queue_len = FMQ_QUEUE_SIZE((FMQ_Queue*)queue);

    if (node == NULL)
        queue_is_empty = true;

    json_object_set_new(root, "queue_empty", json_boolean(queue_is_empty));
    json_object_set_new(root, "queue_length", json_integer(queue_len));
    json_object_set_new(root, "status", json_string("OK"));
    json_object_set_new(root, "request_start", json_string(asctime(req_start)));
    time(&rawtime_end);
    req_end = localtime(&rawtime_end);
    json_object_set_new(root, "request_end", json_string(asctime(req_end)));
    ulfius_set_json_body_response(response, 200, json_pack("o*", root));
    json_decref(root);
    return U_CALLBACK_CONTINUE;
}

static int start_server(FMQ_TCP *tcp)
{
    struct _u_instance instance;

    if (ulfius_init_instance(&instance, tcp->port, NULL, NULL) != U_OK)
    {
        fprintf(stderr, "Error starting ulfius server\n");
        exit(EXIT_FAILURE);
    }

    ulfius_add_endpoint_by_val(&instance, "POST", "/consumer", NULL, 0, &callback_consumer, tcp->queue);
    ulfius_add_endpoint_by_val(&instance, "POST", "/provider", NULL, 0, &callback_provider, tcp->queue);
    ulfius_add_endpoint_by_val(&instance, "GET", "/health", NULL, 0, &callback_health, tcp->queue);
    if (ulfius_start_framework(&instance) == U_OK)
    {
        printf("Starting server on http://localhost:%d\n", instance.port);
        while(1)
        {
            // block main thread
        }
    }
    else
    {
        fprintf(stderr, "Error starting server...\n");
        printf("Error: \n\t- Tip: Check that port %d is not busy\n", tcp->port);
    }
    FMQ_LOGGER(tcp->log_level, "Stopping server \n");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
    return 0;
}

FMQ_TCP *FMQ_TCP_new(FMQ_Queue *queue, const u_int16_t port, const int8_t log_level,
    bool run_as_daemon)
{
    FMQ_TCP *tcp = (FMQ_TCP*)malloc(sizeof(FMQ_TCP));
    tcp->queue = queue;
    tcp->start = start_server;
    tcp->port = port;
    tcp->log_level = log_level;
    tcp->run_as_daemon = run_as_daemon;
    return tcp;
}
