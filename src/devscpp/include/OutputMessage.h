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
#ifndef DEVS_CPP_OUTPUT_MESSAGE__
#define DEVS_CPP_OUTPUT_MESSAGE__

#include "Message.h"

namespace DEVS {

class OutputMessage : public Message
{
public:

    OutputMessage(Port srcPort): Message(srcPort)
    {}

    OutputMessage(char* buffer, unsigned int size) : Message( buffer, size )
    {} 

    virtual ~OutputMessage() 
    {}


    /* For an output message, we only have information about the source port that
       was specified by the model */
    Port srcPort() const { return port_; }
}; // OutputMessage

}; // DEVS

#endif

