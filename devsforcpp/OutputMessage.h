/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
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


    /* De un mensaje de salida solo se conoce el puerto de origen 
       que fue especificado por el modelo */
    Port srcPort() const { return port_; }
}; // OutputMessage

}; // DEVS

#endif

