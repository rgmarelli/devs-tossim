/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_MESSAGE__
#define DEVS_CPP_MESSAGE__

#include "Buffer.h"
#include "Port.h"

namespace DEVS {

class Message : public Buffer
{
public:

    Message(Port port) : Buffer(), port_(port)
    {}

    Message(char* buffer, unsigned int size) : Buffer() {
        this->fromBuffer( buffer, size );
    }

    virtual ~Message() {
    }
   
    unsigned int serialize(char* buffer) const {
        // port_size|port|content_size|content
        unsigned int total_data_size = 0;
        unsigned int port_size = port_.serialize( &buffer[ sizeof(unsigned int) ] );
        memcpy( &buffer[0], &port_size ,sizeof(unsigned int) );
        if(content_size_ > 0) {
            memcpy( &buffer[ sizeof(unsigned int) + port_size ], &content_size_, sizeof(unsigned int)); 
            memcpy( &buffer[ 2*sizeof(unsigned int) + port_size ], content_, content_size_); 
            total_data_size = port_size + 2*sizeof(unsigned int) + content_size_;
        }
        else {
            total_data_size = sizeof(unsigned int) + port_size;
        }
        return total_data_size;
    }

    virtual void fromBuffer(char* buffer, unsigned int size) {
        unsigned int port_size=0;
        memcpy(&port_size, &buffer[0], sizeof(unsigned int));
        port_.fromBuffer( &buffer[sizeof(unsigned int)], port_size );
        if( size > (port_size + sizeof(unsigned int) ) ) {
            unsigned int content_size;
            memcpy(&content_size, &buffer[sizeof(unsigned int)+port_size], sizeof(unsigned int));
            putContent(&buffer[2*sizeof(unsigned int)+port_size], content_size);    
        }
    }

protected:
    Port port_;

}; // Message

}; // DEVS

#endif

