//
// Created by Joe Gasewicz on 07/10/2024.
//

#include <stdlib.h>
#include <string.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <jansson.h>
#include <microhttpd.h>
#include "server.h"
#include "config.h"
#include "utils.h"

/**
 * Gets the request's body as a pointer to a string & stores in body_data
 * @param json_body The caller is required to free body_data
 * @param req libevents requests
 * @param q pointer to FMQ_Queue
 */
static void get_request_body(char *json_body, struct evhttp_request *req, FMQ_Queue *q)
{
    // network operation
    struct evbuffer *input_buffer = evhttp_request_get_input_buffer(req);
    size_t body_len = evbuffer_get_length(input_buffer);
    // allocate memory to store the request body
    char *body_data = malloc(body_len + 1);
    if (body_data)
    {
        evbuffer_copyout(input_buffer, body_data, body_len);
        body_data[body_len] = '\0';
        memcpy(json_body, body_data, body_len);
        free(body_data);
    }
}

static void consumer_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue)
{
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    const FMQ_QNode *node = FMQ_Queue_dequeue((FMQ_Queue*)queue);
    if (node == NULL)
    {
        FMQ_LOGGER(q->log_level ,"{consumer}: Queue is empty\n");
        json_t *root = json_object();
        json_object_set_new(root, "error", json_string("Queue is empty!"));
        char *json_str = json_dumps(root, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(root);
        return;
    }
    const FMQ_Data *dataPtr = (FMQ_Data*)node->data;
    json_t *message_load = json_loads(dataPtr->message, JSON_ENCODE_ANY, NULL);
    if (json_is_null(message_load))
    {
        const char *err_msg = "{consumer}: Error: No message in stored queue node.";
        json_t *err_json = json_object();
        json_object_set_new(err_json, "error", json_string(err_msg));
        FMQ_LOGGER(q->log_level, "{consumer}: Error: No message in stored queue node.\n");
        char *json_str = json_dumps(err_json, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(err_json);
        return;
    }
    char *msg_dump = json_dumps(message_load, JSON_COMPACT);
    FMQ_LOGGER(q->log_level, "{consumer}: Successfully dequeued message for consumer\n");
    FMQ_LOGGER(q->log_level, "{consumer}: Received: %s\n", msg_dump);
    free(msg_dump);
    free((FMQ_Data*)node->data);
    free((FMQ_QNode*)node);
    json_t *json_res_object = json_object();
    JSON_INDENT(4);
    json_object_set_new(json_res_object, "status", json_string(q->status));
    json_object_set_new(json_res_object, "queue_length", json_integer(q->size));
    json_object_set_new(json_res_object, "message_size", json_integer(q->msg_size));
    json_object_set_new(json_res_object, "message", message_load);
    char *json_str = json_dumps(json_res_object, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
    free(json_str);
    json_decref(message_load);
    json_decref(json_res_object);
}

static void provider_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue)
{
    char body_data[FMQ_MESSAGE_SIZE]; // buffer
    FMQ_Queue *q = (FMQ_Queue*)queue;
    json_t *json_res_object = NULL;
    get_request_body(body_data, req, q);
    // turn the request body's data into a jansson JSON object
    json_error_t error;
    json_t *json_req_object = json_loads(body_data, JSON_INDENT(4), &error);
    // get JSON object's values
    json_t *message = json_object_get(json_req_object, "message");
    if (message == NULL)
    {
        FMQ_LOGGER(q->log_level, "{provider} ERROR: No JSON in request body\n");
        json_t *root = json_object();
        json_object_set_new(root, "error", json_string("expected 'message' key in JSON"));
        char *json_str = json_dumps(root, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(root);
        json_decref(json_req_object);
        return;
    }
    const bool destroy = json_boolean_value(json_object_get(json_req_object, "destroy"));
    if (destroy) {
        FMQ_LOGGER(q->log_level, "{provider}: Successfully destroyed queue\n");
        char *json_str = json_dumps(message, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(message);
        json_decref(json_req_object);
        return;
    }
    // Handle successful request & do queue operations
    char *message_dump = json_dumps(message, JSON_COMPACT);
    FMQ_LOGGER(q->log_level, "{provider}: Received message successfully\n");
    FMQ_Data *data = (FMQ_Data*)malloc(sizeof(FMQ_Queue));
    data->message = malloc(sizeof(char) * q->msg_size);
    // json_dumps must be freed before we can deallocate message
    char *json_q_dump = json_dumps(message, JSON_COMPACT);
    strcpy(data->message, json_q_dump);
    free(json_q_dump);
    FMQ_Queue_enqueue((FMQ_Queue*)queue, data);

    // Prepare response
    json_res_object = json_object();
    json_object_set_new(json_res_object, "status", json_string(q->status));
    json_object_set_new(json_res_object, "queue_length", json_integer(q->size));
    json_object_set_new(json_res_object, "message_size", json_integer(q->msg_size));
    json_object_set_new(json_res_object, "message", json_pack("o*", message));

    char *json_res_str = json_dumps(json_res_object, JSON_INDENT(4));
    // clean up
    json_decref(json_req_object);
    json_decref(message);
    free(message_dump);
    json_decref(json_res_object);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
    evbuffer_add_printf(reply,  json_res_str);
#pragma GCC diagnostic pop
    free(json_res_str);
}


static char* get_req_method(enum evhttp_cmd_type cmd)
{
    switch (cmd) {
        case EVHTTP_REQ_GET: return "GET";
        case EVHTTP_REQ_POST: return "POST";
        case EVHTTP_REQ_HEAD: return "HEAD";
        case EVHTTP_REQ_PUT: return "PUT";
        case EVHTTP_REQ_DELETE: return "DELETE";
        case EVHTTP_REQ_OPTIONS: return "OPTIONS";
        case EVHTTP_REQ_TRACE: return "TRACE";
        case EVHTTP_REQ_CONNECT: return "CONNECT";
        case EVHTTP_REQ_PATCH: return "PATCH";
        default: return "unknown";
    }
}

static void resp_callback(struct evhttp_request *req, void *queue)
{
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    struct evbuffer *reply = evbuffer_new();     // <-- Allocate storage for a new evbuffer.

    const char *request_uri = evhttp_request_get_uri(req);
    const char *request_host = evhttp_request_get_host(req);
    const char *allowed_hosts[FMQ_ALLOWED_HOSTS_LENGTH] = {"localhost", "0.0.0.0", "127.0.0.1"};
    const bool host_allowed = check_allowed_hosts(request_host, allowed_hosts);

    struct evhttp_uri *parsed_uri = evhttp_uri_parse(request_uri);
    if (!parsed_uri)
    {
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    enum evhttp_cmd_type cmd = evhttp_request_get_command(req);
    const char *method = get_req_method(cmd);
    if (strcmp(method, "unknown") == 0)
    {
        FMQ_LOGGER(q->log_level, "Host:%s not allowed\n", request_host);
        evhttp_send_reply(req, MHD_HTTP_NOT_ACCEPTABLE, "Method not recognized", reply);
    }
    else if (host_allowed == false)
    {
        evhttp_send_reply(req, 403, "Host not allowed", reply);
    }
    else
    {
        FMQ_LOGGER(q->log_level, "{server}: %s %s HTTP/1.1 \n",  method, request_uri);

        // check the method & path
        if (strcmp(method, "POST") == 0 && strcmp(request_uri, "/provider") == 0)
        {
            provider_callback(req, reply, queue);

        }
        else if (strcmp(method, "POST") == 0 && strcmp(request_uri, "/consumer") == 0)
        {
            consumer_callback(req, reply, queue);
        }
        else if (strcmp(method, "GET") == 0 && strcmp(request_uri, "/health") == 0)
        {
            evbuffer_add_printf(reply, "{\"status\":\"OK\"}");
        }
        else
        {
            evbuffer_add_printf(reply, "NULL"); // TODO 404
        }


        struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
        evhttp_add_header(headers, "Content-Type", "application/json");
        evhttp_send_reply(req, HTTP_OK, "OK", reply);
    }
    // cleanup
    if ( parsed_uri )
    {
        evhttp_uri_free(parsed_uri);
    }
    evbuffer_free(reply);
}

static void my_signal_event_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event_base *base = (struct event_base *) arg;
    event_base_loopbreak(base);
}
static int start_server(FMQ_Server *s)
{
    char http_addr[] = "0.0.0.0";
    ev_uint16_t http_port = (ev_uint16_t)s->port;

    struct event_base *base = event_base_new();
    struct evhttp *http_server = evhttp_new(base);

    int http_handle = evhttp_bind_socket(http_server, (const char *) http_addr, http_port);

    // evhttp_set_max_body_size() TODO
    // handlers
    evhttp_set_gencb(http_server, resp_callback, s->queue);
    // Add SIGINT event
    struct event *sig_int = evsignal_new(base, SIGINT, my_signal_event_cb, base);
    event_add(sig_int, NULL);

    FMQ_LOGGER(s->log_level, "Listening request on http://%s:%d (%d)\n", http_addr, http_port, http_handle);
    event_base_dispatch(base);

    evhttp_free(http_server);
    event_free(sig_int);
    event_base_free(base);
    return 0;
}

FMQ_Server *FMQ_Server_new(FMQ_Queue *queue, const uint16_t port,
                       const int8_t log_level, bool run_as_daemon)
{
    FMQ_Server *server = (FMQ_Server*)malloc(sizeof(FMQ_Server));
    server->queue = queue;
    server->start = start_server;
    server->port = port;
    server->log_level = log_level;
    server->run_as_daemon = run_as_daemon;
    return server;
}
