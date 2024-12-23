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
 * @param allowed_hosts_len
 * @return returns true if a single host matches the incoming requests host
 */
bool check_allowed_hosts(const char *request_host, char **allowed_hosts, int allowed_hosts_len)
{
    for (int i = 0; i < allowed_hosts_len; i++)
    {
        if (strcmp(request_host, allowed_hosts[i]) == 0)
        {
            return true;
        }
    }
    return false;
}
