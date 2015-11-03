/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_REMOTE_SIMULATOR_CONNECTOR__
#define DEVS_CPP_REMOTE_SIMULATOR_CONNECTOR__

#include "RemoteMessageInterface.h"
#include "RemoteMessage.h"
#include "Simulator.h"
#include <memory>

namespace DEVS {

template <class MESSAGE_INTERFACE>
class RemoteSimulatorConnector
{
public:

    RemoteSimulatorConnector( Simulator* simulator, Buffer properties=Buffer() ) : peer_(NULL), simulator_(simulator), properties_(properties)
    {}

    virtual ~RemoteSimulatorConnector() {
        delete(peer_);
    }

    bool connect(std::string connection_string) {
        MESSAGE_INTERFACE message_interface;
        this->peer_ = message_interface.connect( connection_string );
        if( this->peer_ == NULL ) {
            Log::write(LOG_ERR,"DEVS", "[%s] RemoteSimulatorConnector. Error connecting to: %s", simulator_->model_name().c_str(), connection_string.c_str());
            return false;
        }

        /* Send connection message */
        unsigned int payload_size = simulator_->model_name().size() + 1 + properties_.size();
        char payload[payload_size];
        memcpy( payload, simulator_->model_name().c_str(), simulator_->model_name().size() );
        payload[simulator_->model_name().size()] = '\0';
        if( properties_.size() > 0 ) {
            memcpy( &payload[simulator_->model_name().size()+1], properties_.content(), properties_.size() );
        }
        
        RemoteMessage connect_req( RemoteMessage::TYPE_CONNECTION_REQUEST, simulator_->nextTN(), 
                                   payload, payload_size );
        
        if( message_interface.send( peer_, connect_req ) == -1 ) {
            Log::write(LOG_ERR,"DEVS", "[%s] RemoteSimulatorConnector. Error sending to: %s", simulator_->model_name().c_str(), connection_string.c_str());
            return false;
        }

        std::auto_ptr<RemoteMessage> connect_rep( message_interface.recv( peer_ ) );
        if( connect_rep.get() != NULL ) {
             Log::write(LOG_INFO,"DEVS", "[%s] RemoteSimulatorConnector. connection done=%d ", simulator_->model_name().c_str(), connect_rep->type());
            return true;
        }
        return false;
    }

    RemoteMessage* process(RemoteMessage* m)
    {
        RemoteMessage *output=NULL;
        switch(m->type())
        {
            case RemoteMessage::TYPE_INTERNAL_TRANSITION_REQUEST:
            {
                OutputMessage* output_message = simulator_->internalTransition(m->nextTN());
                output = new RemoteMessage( RemoteMessage::TYPE_INTERNAL_TRANSITION_REPLY, simulator_->nextTN(), output_message );
                delete(output_message);
                break;
            }
            case RemoteMessage::TYPE_EXTERNAL_TRANSITION_REQUEST:
            {
                ExternalMessage *external_message = m->getMessage<ExternalMessage>();
                simulator_->externalTransition(m->nextTN(), external_message);
                output = new RemoteMessage( RemoteMessage::TYPE_EXTERNAL_TRANSITION_REPLY, simulator_->nextTN(), NULL );
                break;
            }
            case RemoteMessage::TYPE_DISCONNECT_REQUEST:
            {
                output = new RemoteMessage( RemoteMessage::TYPE_DISCONNECT_REPLY, simulator_->nextTN(), NULL );
                break;
            }
        }
        return output;
    }

    bool doProtocol() {
        if( this->peer_ == NULL ) {
            Log::write(0, "DEVS", "[%s] RemoteSimulatorConnector. Error: not connected", simulator_->model_name().c_str());  
            return false;
        }

        MESSAGE_INTERFACE message_interface;

        /* Hacer el protocolo request-response */   
 
        std::auto_ptr<RemoteMessage> request( message_interface.recv( peer_ ) );
        if( !request.get() ) {
            Log::write(0, "DEVS", "[%s] RemoteSimulatorConnector. Error: recv error", simulator_->model_name().c_str()); 
            return false;
        }

        std::auto_ptr<RemoteMessage> response( this->process( request.get() ) );
        if(response.get() != NULL) {
            message_interface.send( peer_, *response );
        }

        if(request->type() == RemoteMessage::TYPE_DISCONNECT_REQUEST) {
            message_interface.disconnect(peer_);
            return false;
        }
        return true;
    }

protected:
    typename MESSAGE_INTERFACE::type_peer *peer_;

private:
    Buffer properties_;
    Simulator* simulator_;
}; // RemoteSimulatorConnector

}; // DEVS

#endif


