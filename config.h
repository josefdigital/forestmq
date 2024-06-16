//
// Created by Joe Gasewicz on 15/06/2024.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <stdarg.h>
#include <stdio.h>
#define FMQ_DEBUG 1 // TODO unset for production builds

#define FMQ_LOGGER(...) \
{ \
  if (FMQ_DEBUG) \
  { \
    printf(__VA_ARGS__);  \
  } \
}\

#endif //CONFIG_H
