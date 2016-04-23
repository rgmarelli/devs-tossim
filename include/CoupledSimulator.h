/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_COUPLED_SIMULATOR__
#define DEVS_CPP_COUPLED_SIMULATOR__

#include "EventQueue.h"
#include "Simulator.h"
#include "CoupledModel.h"
#include <iostream>

namespace DEVS {

class CoupledSimulator : public Simulator
{
public:
    typedef std::map<std::string,Simulator*> SimulatorMap;

public:
    CoupledSimulator(CoupledModel *model) : model_(model)
    {
    }
 
    void simulate() {
        TIME tn = this->nextTN();
        if( tn == DEVS::TIME::infinity() ) {
            Log::write(LOG_NOTICE,"DEVS","[%s] nextTN of model is infinity",model_->name().c_str());
            return;
        }
        this->internalTransition(tn);
    }  

    void addSimulator(Simulator *simulator) {
        childs_.insert(std::pair<std::string,Simulator*>(simulator->model_name(),simulator));
        queue_.setInternalEvent(simulator->nextTN(),simulator);
    }

    void dump() const {
        queue_.dump();
    }

    OutputMessage* internalTransition(TIME t) {
        OutputMessage* myOutputMessage=NULL;
        if( queue_.size() == 0 ) {
            Log::write(LOG_WARNING, "DEVS", "[%s] EventQueue is empty.", model_->name().c_str());
            return NULL;
        }

        if(t == nextTN()) {
            Event event = queue_.pop();
            Simulator* simulator = event.simulator();
            if(simulator != NULL) {
                /* Notar que applyDeltFunction no es necesaria porque con la cola de eventos
                   conozco si tengo que realizar una transicion externa o una interna.
                 */
                if(event.internal()) {
                    OutputMessage *message = simulator->internalTransition(event.TN());
                    if(message != NULL) {
                        myOutputMessage = processOutputMessage(event.TN(),message);
                        delete(message);
                    }
                }
                else {
                    // Mensaje entre simuladores hermanos
                    simulator->externalTransition(event.TN(),event.message());
                }
                if( simulator->error() ) {
                    this->handle_simulator_error( simulator );
                }
                else {
                    queue_.setInternalEvent(simulator->nextTN(),simulator);
                }
            }
            else {
                Log::write(LOG_ERR, "DEVS", "[%s] Simulator for event is NULL.", model_->name().c_str());
            }
        }
        else {
            Log::write(LOG_ERR, "DEVS", "[%s] T != nextTN.", model_->name().c_str());
        }
        return myOutputMessage;
    }
 
    void externalTransition(TIME t, ExternalMessage* message) {
        /* Acomplamiento de entrada externa que conecta los ports de entrada
           del modelo acoplado a uno o mas de los ports de los componentes. (wainer) */
        CoupledModel::PortList portList = model_->couplings(message->dstPort());
        if(portList.size() > 0) {
            CoupledModel::PortList::iterator it = portList.begin();
            while(it != portList.end()) {

                Simulator* simulator = getSimulatorByInputPort(*it);
                ExternalMessage* myMessage = new ExternalMessage(*it);
                myMessage->putContent(message->content(),message->size());
                simulator->externalTransition(t,myMessage);
                if( simulator->error() ) {
                    this->handle_simulator_error( simulator );
                }
                else {
                    queue_.setInternalEvent(simulator->nextTN(),simulator);
                }
                it++;
            }
        }
        else {

        }

        delete(message);
    }

    TIME nextTN() const {
        if( queue_.empty() ) {
            return DEVS::TIME::infinity();
        }
        return queue_.front().TN();
    }

    virtual std::string model_name() const {
        return model_->name();
    }

    const SimulatorMap& childs() const { 
        return childs_;  
    }

    const unsigned int queue_size() const { 
        return queue_.size(); 
    }

protected:
    virtual void handle_simulator_error( Simulator *simulator ) {
        Log::write(LOG_ERR,"DEVS", "[%s] Simulator error for model %s", model_name().c_str(), simulator->model_name().c_str()); 
        Log::write(LOG_WARNING,"DEVS", "[%s] Removing child model %s...", model_name().c_str(), simulator->model_name().c_str()); 
        
        this->remove_simulator( simulator );
        this->remove_model( simulator->model_name() );
    }

    virtual void remove_simulator( Simulator *simulator ) {
        queue_.removeSimulator( simulator );

        SimulatorMap::iterator it = childs_.find( simulator->model_name() );
        if( it != childs_.end() ) {
            childs_.erase( it );
        }
    }

    virtual void remove_model( std::string model_name ) {
        model_->remove( model_name );
    }

private:

    Simulator* getSimulatorByInputPort(Port port) {
        SimulatorMap::iterator it = childs_.find(port.model_name());     
        if(it != childs_.end()) {
            return it->second; 
        }
        return NULL;
    }


    mutable EventQueue queue_; /* mutable: Para poder llamarlo desde nextTN() que debe ser const */
    SimulatorMap childs_;

    CoupledModel *model_;
    ExternalMessage *message_;

    bool hasMessage() const {
        return (message_ != NULL);
    }

    OutputMessage* processOutputMessage(TIME t, OutputMessage *message)
    {  
        OutputMessage* myOutputMessage=NULL;
        // El mensaje es para un simulador hermano del actual o para el padre.
        CoupledModel::PortList portList = model_->couplings(message->srcPort());
        Log::write(LOG_DEBUG,"DEVS","[%s] processOutputMessage = %s", model_name().c_str(), message->srcPort().name().c_str());
        if(portList.size() > 0) {
            CoupledModel::PortList::iterator it = portList.begin();
            while(it != portList.end()) {
                 if(model_->outputPorts().hasPort(*it)) {
                     /* El puerto esta conectado a un puerto de salida. */
                     myOutputMessage = new OutputMessage(model_->outputPorts().getPort(*it));
                     myOutputMessage->putContent(message->content(),message->size());
                 }
                 else {
                     Simulator* simulator = getSimulatorByInputPort(*it);
                     if(simulator == NULL) {
                         Log::write(LOG_ERR,"DEVS","[%s] addExternalEvent no simulator for coupling [%s:%s] -> [%s:%s]",
                                                    model_name().c_str(),
                                                    message->srcPort().model_name().c_str(),
                                                    message->srcPort().name().c_str(),
                                                    it->model_name().c_str(),
                                                    it->name().c_str() );
                     }
                     else {
                         ExternalMessage* myMessage = model_->translate( message, *it );
                         if( myMessage != NULL ) {
                            queue_.addExternalEvent(t,simulator,myMessage);   
                         }
                     }                             
                 }
                 it++;
            }
        }
        else {
            Log::write(LOG_WARNING,"DEVS", "[%s] No couplings for the src port %s:%s. Dropping message.",
                                            model_name().c_str(), 
                                            message->srcPort().model_name().c_str(), message->srcPort().name().c_str());
        }
        return myOutputMessage;
    }

   
    virtual Model* model() const {
        return model_;
    }
}; // CoupledSimulator

}; // DEVS

#endif


