#include <stdio.h>
#include <stdlib.h>
#include <ulfius.h>

#define PORT 8005

int callback_consumer(const struct _u_request *request,
    struct _u_response *response, void *user_data)
{
    ulfius_set_string_body_response(response, 200, "Consumer...");
    return U_CALLBACK_CONTINUE;
}

int main(void)
{
    struct _u_instance instance;

    if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK)
    {
        fprintf(stderr, "Error starting ulfius server\n");
        exit(EXIT_FAILURE);
    }
    // declare endpoints
    ulfius_add_endpoint_by_val(&instance, "GET", "/", NULL, 0, &callback_consumer, NULL);
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
