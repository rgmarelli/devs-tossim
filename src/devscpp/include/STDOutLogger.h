/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_STDOUT_LOGGER__
#define DEVS_CPP_STDOUT_LOGGER__

#include "Logger.h"
#include <stdio.h>
#include <stdarg.h>

namespace DEVS {

class STDOutLogger : public Logger
{
public:

    virtual void log(int priority, const char* module, const char* format, va_list args) {
        char buffer[1024];
        vsprintf(buffer, format, args);
        printf("[%s] %s\n",module,buffer);
        fflush(stdout);
    }
};

} //DEVS

#endif //DEVS_CPP_STDOUT_LOGGER__

