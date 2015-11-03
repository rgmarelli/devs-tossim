/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_REMOTE_BINDING__
#define DEVS_CPP_REMOTE_BINDING__

#include "RemoteSimulator.h"
#include "CoupledSimulator.h"
#include "CoupledModel.h"
#include <list>

namespace DEVS {

template <class MESSAGE_INTERFACE, class SIMULATION>
class RemoteBinding
{
public:
    RemoteBinding( RemoteSimulatorAcceptor<MESSAGE_INTERFACE> &acceptor,
                   SIMULATION &simulation ) : acceptor_(acceptor), simulation_(simulation)
    {}

    void run() {
        pthread_t thread;
        pthread_create(&thread, NULL, &RemoteBinding<MESSAGE_INTERFACE, SIMULATION>::loop, (void*)this);
    }

    void stop() {

    }

    void remove_all() {
        typename std::list<RemoteSimulator<MESSAGE_INTERFACE>*>::iterator it = remote_simulators_.begin();
        for( ; it != remote_simulators_.end(); it++ ) {
            (*it)->disconnect();
            delete( *it );
        }
        remote_simulators_.clear();
    }

protected:
    
    static void* loop( void* arg ) {
        RemoteBinding<MESSAGE_INTERFACE, SIMULATION> *bind = (RemoteBinding<MESSAGE_INTERFACE, SIMULATION>*)arg;
        while( 1 ) {
            RemoteSimulator<MESSAGE_INTERFACE>* handler = bind->acceptor_.accept();
            if( handler == NULL ) {
                return NULL;
            }

            if( bind->simulation_.bind_simulator( handler, handler->properties() ) ) {
                bind->remote_simulators_.push_back( handler );
            }
            else {
                handler->disconnect();
                delete( handler );
            }
        }
        return NULL;
    }

private:
    RemoteSimulatorAcceptor<MESSAGE_INTERFACE> &acceptor_;
    SIMULATION &simulation_;
    std::list<RemoteSimulator<MESSAGE_INTERFACE>*> remote_simulators_;
}; //RemoteBinding

} // DEVS

#endif


