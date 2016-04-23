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

