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
#ifndef DEVS_CPP_REMOTE_SIMULATOR_ACCEPTOR__
#define DEVS_CPP_REMOTE_SIMULATOR_ACCEPTOR__

#include "RemoteMessageInterface.h"
#include "RemoteSimulator.h"
#include "RemoteMessage.h"
#include <memory>

namespace DEVS {

template <class MESSAGE_INTERFACE>
class RemoteSimulatorAcceptor
{
public:

    RemoteSimulatorAcceptor() : acceptor_(NULL)
    {}

    virtual ~RemoteSimulatorAcceptor() {
         delete(acceptor_);
    }

    bool listen(std::string connection_description) {
        this->acceptor_ = MESSAGE_INTERFACE().listen(connection_description); 
    }

    void close() {
    }

    RemoteSimulator<MESSAGE_INTERFACE>* accept() {
        if( this->acceptor_ == NULL ) {
            Log::write(0,"DEVS::RemoteSimulatorAcceptor", "Error: not listening");
            return NULL;
        }

        MESSAGE_INTERFACE message_interface;
        typename MESSAGE_INTERFACE::type_peer* peer = message_interface.accept(acceptor_);

        // Receive a connection message
        std::auto_ptr<RemoteMessage> connect_req( message_interface.recv( peer ) );
        if( !connect_req.get() ) {
            return NULL;
        }

        std::string name( connect_req->payload() );
        Buffer properties;
        if( name.size() < connect_req->payload_size() -1 ) {
            properties.putContent( connect_req->payload()+name.size()+1, connect_req->payload_size() - name.size() - 1 );
        }
        Log::write(0,"DEVS::RemoteSimulatorAcceptor", "Simulator connected. Model name: %s, properties size: %d", name.c_str(), properties.size());

        // Send the reply message */
        RemoteMessage connect_rep( RemoteMessage::TYPE_CONNECTION_REPLY, 0, NULL );
        message_interface.send( peer, connect_rep );    

        return new RemoteSimulator<MESSAGE_INTERFACE>( name, connect_req->nextTN(), peer, properties );
    }
private:
    typename MESSAGE_INTERFACE::type_acceptor *acceptor_;
    
}; // RemoteSimulatorAcceptor

}; // DEVS

#endif


