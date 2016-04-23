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

