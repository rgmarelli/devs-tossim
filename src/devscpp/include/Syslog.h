/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_SYSLOG_LOGGER__
#define DEVS_CPP_SYSLOG_LOGGER__

#include <stdio.h>
#include <syslog.h>

namespace DEVS {

class Syslog : public Logger
{
public:

    Syslog() {
        openlog("DEVSTOSSIM", LOG_PID, LOG_USER);
    }

    virtual ~Syslog() {
        closelog();
    }

    void priority( int p ) {
        setlogmask( p );
    }

    virtual void log(int priority, const char* module, const char* format, va_list args) {
        char buffer[1024];
        vsprintf(buffer, format, args);
        syslog(priority,"[%s] %s",module,buffer);
    }
};

} //DEVS

#endif //DEVS_CPP_SYSLOG_LOGGER__

