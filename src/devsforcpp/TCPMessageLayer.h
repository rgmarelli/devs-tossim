/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_TCPMESSAGELAYER__
#define DEVS_CPP_TCPMESSAGELAYER__

#include "BasicTCPMessageLayer.h"
#include "RemoteMessageInterface.h"

#include <iostream>
#include <string>
#include <sstream>

namespace DEVS {

class TCPMessageLayer : public RemoteMessageInterface<Socket,Socket>
{
public:

    virtual type_acceptor* listen(std::string connection_description) {
        Connection_Info info = this->parse_connection_descripion( connection_description );
        return tcp_.listen( info.port );
    }

    virtual type_peer* accept(type_acceptor* acceptor) {
        return tcp_.accept( acceptor );
    }

    virtual void close(type_acceptor* acceptor) {
        tcp_.close( acceptor );
    }

    virtual type_peer* connect(std::string connection_description) {
        Connection_Info info = this->parse_connection_descripion( connection_description );
        return tcp_.connect( info.host.c_str(), info.port );
    }

    virtual int send(type_peer* peer, RemoteMessage &message) {

        unsigned int max_size = RemoteMessage::max_message_size();
        unsigned char buffer[max_size];
        memcpy( buffer, message.raw_data(), message.raw_size() );

        return tcp_.send( peer, buffer, max_size );
    }

   virtual RemoteMessage* recv(type_peer* peer) {

        unsigned int max_size = RemoteMessage::max_message_size();
        unsigned char buffer[max_size];

        int total=tcp_.recv( peer, buffer, max_size );
        if( total <= 0 ) {
            return NULL;
        }
        return new RemoteMessage( buffer, total );
    }

    virtual void disconnect(type_peer* peer) {
        return tcp_.disconnect( peer );
    }

private:
    struct Connection_Info {
        unsigned int port;
        std::string host;
    };

    Connection_Info parse_connection_descripion( std::string connection_description ) {
        Connection_Info info;

        std::stringstream ss(connection_description);
        std::string port_str;

        std::getline(ss, info.host, ':');
        std::getline(ss, port_str, ':');

        if( port_str.empty() ) {
            info.port = 0;
        }
        else {
            info.port = atoi(port_str.c_str());
        }
        return info;    
    }

    BasicTCPMessageLayer tcp_;
}; 

} //DEVS

#endif //DEVS_CPP_TCPMESSAGELAYER__
