/* 
 * DEVS-TOSSIM - a DEVS framework for simulation of TinyOS wireless sensor networks
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
