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
#ifndef DEVS_CPP_REMOTE_SIMULATOR__
#define DEVS_CPP_REMOTE_SIMULATOR__

#include "RemoteMessage.h"
#include "RemoteMessageInterface.h"
#include "Simulator.h"
#include <string>
#include <memory>

namespace DEVS {

template <class MESSAGE_INTERFACE>
class RemoteSimulator : public Simulator // Handler
{
public:
    RemoteSimulator(std::string model_name, TIME nextTN, typename MESSAGE_INTERFACE::type_peer* peer, Buffer properties ) : 
                                                                                                        nextTN_(nextTN),
                                                                                                        model_name_(model_name),
                                                                                                        peer_(peer),
                                                                                                        properties_(properties)
    {}

    virtual ~RemoteSimulator() {
        delete(peer_);
    }

    virtual std::string model_name() const { 
        return model_name_; 
    }

    virtual void externalTransition(TIME t, ExternalMessage* message) {
        RemoteMessage m( RemoteMessage::TYPE_EXTERNAL_TRANSITION_REQUEST, t, message);
        RemoteMessage *output = doProtocol( &m );
        if( output == NULL ) {
            Log::write(LOG_ERR,"DEVS::RemoteSimulator", "[%s] externalTransition. Communication error: passivating model.", model_name().c_str());
            nextTN_ = Time::infinity();
            this->error( true );
            return;
        }
        nextTN_ = output->nextTN();
        delete(output);
    }

    virtual OutputMessage* internalTransition(TIME t) {
        RemoteMessage m( RemoteMessage::TYPE_INTERNAL_TRANSITION_REQUEST, t, NULL);
        RemoteMessage *output = doProtocol( &m );
        if( output == NULL ) {
            Log::write(LOG_ERR,"DEVS::RemoteSimulator", "[%s] internalTransition. Communication error: passivating model.", model_name().c_str());
            nextTN_ = Time::infinity();
            this->error( true );
            return NULL;
        }
        nextTN_ = output->nextTN();
        OutputMessage* output_message = output->getMessage<OutputMessage>();
        delete(output);
        return output_message;
    }

    virtual TIME nextTN() const {
        return nextTN_;
    }

    void disconnect() {
        RemoteMessage m( RemoteMessage::TYPE_DISCONNECT_REQUEST, 0, NULL);
        std::auto_ptr<RemoteMessage> discack( this->doProtocol( &m ) );
        MESSAGE_INTERFACE().disconnect( peer_ );
    }

    const Buffer& properties() const {
        return this->properties_;
    }

protected:
    RemoteMessage* doProtocol(RemoteMessage* m) {
        MESSAGE_INTERFACE message_interface;
        //Send the request to the remote simulator
        if( message_interface.send( peer_, *m ) == -1 ) {
            Log::write(LOG_ERR,"DEVS::RemoteSimulator", "[%s] Send error", model_name().c_str());
            return NULL;
        }

        RemoteMessage *mo = message_interface.recv( peer_ ); // Recibir la respuesta del simulador remoto
        if( mo == NULL ) {
            Log::write(LOG_ERR,"DEVS::RemoteSimulator", "[%s] Receive error", model_name().c_str());
            return NULL;
        }
        return mo;
    }

protected:
    TIME nextTN_;
    std::string model_name_;

    typename MESSAGE_INTERFACE::type_peer* peer_;

    Buffer properties_;
}; // RemoteSimulator

} // DEVS

#endif


