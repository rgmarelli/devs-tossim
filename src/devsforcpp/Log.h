/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_LOG__
#define DEVS_CPP_LOG__

#include <Logger.h>
#include <Syslog.h>

namespace DEVS {

class Log
{
public:
    static Logger* instance(Logger *logger = NULL) { 
        static Logger* instance_ = new Syslog();
        if( logger != NULL ) {
            delete( instance_ );
            instance_ = logger;
        }
        return instance_; 
    }

    static void write(int priority, const char* module, const char* format, ...) {
        va_list args;
        va_start(args, format);
        instance()->log( priority, module, format, args );
        va_end(args);
    }
};

} //DEVS

#endif //DEVS_CPP_LOG__
