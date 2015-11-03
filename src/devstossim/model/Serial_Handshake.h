/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_MOTE_SERIAL_HANDSHAKE__
#define DEVS_MOTE_SERIAL_HANDSHAKE__

#include <BasicTCPMessageLayer.h>

class Serial_Handshake
{
public:
    bool operator()( DEVS::Socket *socket ) {
        DEVS::BasicTCPMessageLayer tcp;

        unsigned char msg_init[] = { 'U', '1' };
        tcp.send( socket,  msg_init, sizeof( msg_init) );
    
        unsigned char buffer[2];
        int size = tcp.recv( socket, buffer, 2, true );
        if (size != 2 ) return false;

        return true;
    }
};

#endif //DEVS_MOTE_SERIAL_HANDSHAKE__
