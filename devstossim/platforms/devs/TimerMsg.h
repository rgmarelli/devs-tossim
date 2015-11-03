/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */

#ifndef DEVS_TINYOS_TIMERMSG__
#define DEVS_TINYOS_TIMERMSG__

#include <stdint.h>

typedef struct  {
    uint32_t dt;
    char one_shot;
} TimerMsg;

#endif //DEVS_TINYOS_TIMERMSG__
