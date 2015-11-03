/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_LOGGER__
#define DEVS_CPP_LOGGER__

#include <stdio.h>
#include <stdarg.h>

namespace DEVS {

class Logger {
public:

    virtual void log(int priority, const char* module, const char* format, va_list args) = 0;
};

} //DEVS

#endif //DEVS_CPP__LOGGER__

