/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_REMOTE_MESSAGE_INTERFACE__
#define DEVS_CPP_REMOTE_MESSAGE_INTERFACE__

#include "RemoteMessage.h"

namespace DEVS {

template <class ACCEPTOR, class PEER>
class RemoteMessageInterface
{
public:

    typedef ACCEPTOR type_acceptor; 
    typedef PEER type_peer; 

    virtual type_acceptor* listen(std::string connection_description)=0;
    virtual type_peer* accept(type_acceptor* acceptor)=0;

    virtual void close(type_acceptor* acceptor)=0;

    virtual type_peer* connect(std::string connection_description)=0;

    virtual int send(type_peer* peer, RemoteMessage &message)=0;
    virtual RemoteMessage* recv(type_peer* peer)=0;

    virtual void disconnect(type_peer* peer)=0;
};

}

#endif //DEVS_CPP_REMOTE_MESSAGE_INTERFACE__

