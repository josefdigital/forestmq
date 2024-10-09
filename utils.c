//
// Created by Joe Gasewicz on 09/10/2024.
//
#include <string.h>
#include <stdbool.h>
#include "config.h"
#include "utils.h"

/**
 * Used to check if the current incoming request's host matches the allowed hosts array
 * @param request_host 
 * @param allowed_hosts 
 * @return returns true if a single host matches the incoming requests host
 */
bool check_allowed_hosts(const char *request_host, const char *allowed_hosts[FMQ_ALLOWED_HOSTS_LENGTH])
{
    for (int i = 0; i < FMQ_ALLOWED_HOSTS_LENGTH; i++)
    {
        if (strcmp(request_host, allowed_hosts[i]) == 0)
        {
            return true;
        }
    }
    return false;
}
