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

/**
 * Gets the request's body as a pointer to a string & stores in body_data
 * @param body_data The caller is required to free body_data
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

static void provider_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue)
{
    char body_data[1082]; // buffer
    FMQ_Queue *q = (FMQ_Queue*)queue;
    get_request_body(body_data, req, q);

    FMQ_LOGGER(q->log_level, "JOSN = %s\n", body_data);
    // turn the request body's data into a jansson JSON object
    json_error_t error;
    json_t *json_obj = json_loads(body_data, JSON_INDENT(4), &error);
    // get JSON object's values
    json_t *message = json_object_get(json_obj, "message");
    if (message == NULL)
    {
        FMQ_LOGGER(q->log_level, "ERROR: No JSON in request body\n");
        json_t *root = json_object();
        json_object_set_new(root, "error", json_string("expected 'message' key in JSON"));
        char *json_str = json_dumps(root, JSON_INDENT(4));
        evbuffer_add_printf(reply, json_str);
        json_decref(root);
        return;
    }
    const bool destroy = json_boolean_value(json_object_get(json_obj, "destroy"));
    if (destroy) {
        FMQ_LOGGER(q->log_level, "{provider}: Successfully destroyed queue\n");
        char *json_str = json_dumps(message, JSON_INDENT(4));
        evbuffer_add_printf(reply, json_str);
        return;
    }

    // TODO continue here...
    evbuffer_add_printf(reply, "{\"provider\":\"{}\"}");
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
    struct evhttp_uri *parsed_uri = evhttp_uri_parse(request_uri);
    if (!parsed_uri) {
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    enum evhttp_cmd_type cmd = evhttp_request_get_command(req);
    const char *method = get_req_method(cmd);
    if (strcmp(method, "unknown") == 0)
    {
        evhttp_send_reply(req, MHD_HTTP_NOT_ACCEPTABLE, "Method not recognized", reply);
    }
    else
    {
        printf("%s %s HTTP/1.1 \n",  method, request_uri);

        // check the method & path
        if (strcmp(method, "POST") == 0 && strcmp(request_uri, "/provider") == 0)
        {
            provider_callback(req, reply, queue);

        }
        else if (strcmp(method, "GET") == 0 && strcmp(request_uri, "/consumer") == 0)
        {
            evbuffer_add_printf(reply, "{\"consumer\":\"{}\"}");
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
        evhttp_send_reply(req, HTTP_OK, NULL, reply);
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
