/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_EXTERNAL_MESSAGE__
#define DEVS_CPP_EXTERNAL_MESSAGE__

#include "Message.h"

namespace DEVS {

class ExternalMessage : public Message
{
public:

    ExternalMessage(Port dstPort) : Message(dstPort) 
    {}

    ExternalMessage(char* buffer, unsigned int size) : Message( buffer, size )
    {} 

    virtual ~ExternalMessage() 
    {}

    /* De un mensaje de salida solo se conoce el puerto de destino. 
       Dicho puerto, en el caso de un modelo acoplado, 
       es especificado por la funcion de traduccion correspondiente.

       El cuerpo de origen es siempre desconocido. 
     */
    Port dstPort() const { return port_; }
}; // ExternalMessage

}; // DEVS

#endif

