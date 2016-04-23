/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_ATOMIC_MODEL__
#define DEVS_CPP_ATOMIC_MODEL__

#include "Model.h"
#include "OutputMessage.h"
#include "ExternalMessage.h"
#include "PortCollection.h"
#include "Time.h"

#include "Log.h"

#include <string>
#include <list>
#include <algorithm>

namespace DEVS {

class AtomicModel : public Model
{
public:
    AtomicModel(const std::string name) : Model(name), sigma_(DEVS::TIME::infinity())
    {}

    // Funcion de transicion interna (delta int)
    virtual void internalTransition()=0;

    // Funcion de transicion externa (delta Ext)
    virtual void externalTransition(ExternalMessage* message)=0;

    // Funcion de salida (Y)
    virtual OutputMessage* outputFunction()=0;

    // Helper para poder tener el tiempo de la transición
    virtual void internalTransition(const TIME &t) {
        this->tn_ = t;
        this->internalTransition();
    }

    // Helper para poder tener el tiempo de la transición
    virtual void externalTransition(const TIME &t, ExternalMessage* message) {
        this->tn_ = t;
        this->externalTransition(message);
    }

    // Funcion de duracion de un estado (D)
    virtual TIME timeAdvanceFunction() { 
        // La implementacion por defecto de la funcion devuelve sigma
        // que es el tiempo durante el cual permanezco en la fase actual
        return sigma_;
    }

    std::string phase() { return phase_; }
    bool phaseIs(std::string phase) { return phase == phase_; }
    bool phaseWas(std::string phase) { return phase == previous_phase_; }

protected:
    void setSigma(const TIME &sigma) {
       if( sigma == Time::infinity() ) {
           Log::write(LOG_DEBUG,"DEVS", "[%s] setSigma: infinity", name().c_str());
       }
       else {
           Log::write(LOG_DEBUG,"DEVS", "[%s] setSigma: %s (%ds%dms)", name().c_str(), sigma.to_string().c_str(), sigma.sec(), sigma.msec());
       }
       sigma_ = sigma;
    }

    void setPhase(std::string phase) { 
        if( !phaseIsValid(phase) ) {
            // La fase a la que se quiere ir no esta registrada.
            // Es un error en la definicion del modelo.
            Log::write(LOG_CRIT,"DEVS", "Phase %s not found for model %s",phase.c_str(),name().c_str());
            exit(1);
        }
        
        Log::write(LOG_DEBUG,"DEVS", "[%s] Phase change: %s -> %s",name().c_str(),phase_.c_str(),phase.c_str());
        // Almacenar la fase anterior ya que la funcion de salida depende del estado anterior 
        // a la transicion [WAI01]
        previous_phase_ = phase_; 
        phase_ = phase; 
    }

    void registerPhase(std::string phase) {
        if( !phaseIsValid(phase) ) {
            phaseList.push_back(phase);
        }
    }

    bool phaseIsValid(std::string phase) {
        if ( std::find(phaseList.begin(), phaseList.end(), phase) == phaseList.end() ) { 
            return false;
        }
        return true;
    }

    TIME tn() { return this->tn_; }    

private:
    TIME sigma_; // Tiempo durante el cual permanezco en la fase actual
    TIME tn_; // Tiempo en el que tuvo lugar la ultima transición externa o interna.

    std::string phase_;
    std::string previous_phase_;
    std::list<std::string> phaseList;

}; // AtomicModel

}; // DEVS

#endif
