/* 
 * devsCPP - a DEVS C++ library
 * Copyright (c) 2013 Ricardo Guido Marelli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
