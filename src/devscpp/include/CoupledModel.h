/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_COUPLED_MODEL__
#define DEVS_CPP_COUPLED_MODEL__

#include "ExternalMessage.h"
#include "OutputMessage.h"
#include "Model.h"
#include "Log.h"
#include <map>

namespace DEVS {

class CoupledModel : public Model
{
public:
    typedef std::list<Port> PortList;
    typedef std::list<std::string> ModelList;

public:
    CoupledModel(const std::string name) : Model(name)
    {}

    /* Suponemos que el nombre del modelo es unico dentro del contexto de un modelo acoplado */
    void add(std::string model_name) {
        if( this->name() == model_name ) {
            Log::write(LOG_WARNING,"DEVS","Unable to add child model with the same than the coupled model (%s)",model_name.c_str());
            return;
        }
        if( this->hasModel( model_name ) ) {
            Log::write(LOG_WARNING,"DEVS","Model %s already has a child model with name %s",name().c_str(), model_name.c_str());
            return;
        }



        models_.push_back(model_name);
    }

    void remove(std::string model_name) {
        models_.remove( model_name );
        std::map< Port, PortList >::iterator it = this->couplings_.begin();
        while( it != this->couplings_.end() ) {
            if( it->first.model_name() == model_name ) {
                this->couplings_.erase( it++ );
            }
            else {
                it->second.remove_if( RemoveIf( model_name ) );
                if( it->second.empty() ) {
                    this->couplings_.erase( it++ );
                }
                else {
                    it++;
                }
            }
        }
    }

    ModelList& getModels() {
        return this->models_;
    }

    // Función de traducción (Zij)
    // La implementación por defecto devuelve el mismo mensaje
    virtual ExternalMessage* translate( OutputMessage *message, Port &dstPort ) {
         ExternalMessage* myMessage = new ExternalMessage(dstPort);
         myMessage->putContent(message->content(),message->size());
         return myMessage;
    }

    // Couple srcPort to dstPort.
    virtual bool addCoupling(const Port& srcPort, const Port& dstPort) {

        if( srcPort.empty() || dstPort.empty() ) {
            /* Puertos no encontrados */
            return false;
        }

        if( srcPort.type() != dstPort.type()) {
            /* El tipo de los puertos no coincide, entonces, no permito acoplarlos */
            return false;
        }

        if(!validateCouplingSrcPort(srcPort) || !validateCouplingDstPort(dstPort)) {
            /* Puertos no validos, entonces, no permito acoplarlos */
            return false;
        }

        Log::write(LOG_INFO, "DEVS", "CoupledModel: %s. Add coupling [%s:%s]->[%s:%s]", 
                             name().c_str(),
                             srcPort.model_name().c_str(), srcPort.name().c_str(),
                             dstPort.model_name().c_str(), dstPort.name().c_str());

        /* Registro el acoplamiento */
        couplings_[srcPort].push_back(dstPort);
    }

    // Acoplar srcPort a todos los puertos con el nombre "dstPortName".
    virtual bool addCouplingForAllModels(const Port& srcPort, const std::string &dstPortName) {
        ModelList::const_iterator it = models_.begin();
        while(it != models_.end()) {
            if( *it != srcPort.model_name() ) {
                Port dstPort( *it, dstPortName, srcPort.type() );
                this->addCoupling( srcPort, dstPort );
            }
            it++;
        }
    }

    // Acoplar todos los puertos con el nombre "srcPortName" a dstPort.
    virtual bool addCouplingForAllModels(const std::string &srcPortName, const Port& dstPort) {
        ModelList::const_iterator it = models_.begin();
        while(it != models_.end()) {
            if( *it != dstPort.model_name() ) {
                Port srcPort( *it, srcPortName, dstPort.type() );
                this->addCoupling( srcPort, dstPort );
            }
            it++;
        }
    }

    const PortList& couplings(const Port& port) {
        return couplings_[port];
    }

    bool hasModel(const std::string& model_name) {
        ModelList::const_iterator it = models_.begin();
        while(it != models_.end()) {
            if(*it == model_name) return true;
            it++;
        }
        return false;
    }

    void dump_couplings() {
        std::map< Port, PortList >::iterator it = this->couplings_.begin();
        for( ; it != this->couplings_.end(); it++ ) {
            Log::write(0,"DEVS","[%s:%s]", it->first.model_name().c_str(), it->first.name().c_str());
            for( PortList::iterator portit = it->second.begin(); portit != it->second.end(); portit++ ) {
                Log::write(0,"DEVS","    ->[%s:%s]", portit->model_name().c_str(), portit->name().c_str());
            } 
        }
    }

private:
    class RemoveIf
    {
    public:
        RemoveIf( std::string model ) : model_(model)
        {}

        bool operator()(const Port &p ) {
            return p.model_name() == model_;
        }
    private:
        std::string model_;
    };

private:
    std::map< Port, PortList > couplings_;
    ModelList models_;

    bool validateCouplingSrcPort(const Port& port) {
        if(port.model_name() == name()) {
            return this->inputPorts().hasPort(port);
        }
        if(hasModel(port.model_name())) {
            // Solamente chequeamos si el modelo existe, no sus puertos
            return true;
        }
        return false;
    }

    bool validateCouplingDstPort(const Port& port) {
        if(port.model_name() == name()) {
            return this->outputPorts().hasPort(port);
        }
        if(hasModel(port.model_name())) {
            // Solamente chequeamos si el modelo existe, no sus puertos
            return true;
        }
        return false;
    }

}; // CoupledModel

}; // DEVS

#endif

