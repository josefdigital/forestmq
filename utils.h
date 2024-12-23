//
// Created by Joe Gasewicz on 09/10/2024.
//

#ifndef UTILS_H
#define UTILS_H


bool check_allowed_hosts(const char *request_host, char **allowed_hosts,
    int allowed_hosts_len);

#endif //UTILS_H
