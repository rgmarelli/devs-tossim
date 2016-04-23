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

    // DEVS internal transition (delta int)
    virtual void internalTransition()=0;

    // DEVS external transition (delta Ext)
    // It receives the external message as a parameter. 
    // The memory allocated for this pointer is automatically released by the framework. 
    virtual void externalTransition(ExternalMessage* message)=0;

    // DEVS output message (Y)
    // This function is called to generate an output message that depends on the state of the model. 
    // To return an output message, you must allocate memory that will be released by the framework. 
    // If you do not want to return an output message, this function must return *NULL*. 
    virtual OutputMessage* outputFunction()=0;

    // Helper function used to set the time during the transition
    virtual void internalTransition(const TIME &t) {
        this->tn_ = t;
        this->internalTransition();
    }

    // Helper function used to set the time during the transition
    virtual void externalTransition(const TIME &t, ExternalMessage* message) {
        this->tn_ = t;
        this->externalTransition(message);
    }

    // time advance function (D)
    virtual TIME timeAdvanceFunction() { 
        /* The state variable "sigma" can be assigned invoking the method setSigma(). 
           The method *timeAdvanceFunction()* can be overloaded to provide a different implementation.
           The class TIME represents time in DEVS and allows passivating a model with the constant: 
           DEVS::TIME::infinity(). It has nanosecond precision. */
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
            // The phase is not registered
            // It's an error in the definition of the model
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
    TIME sigma_; //Duration of the current phase
    TIME tn_; //Time of the last internar or external transition

    std::string phase_;
    std::string previous_phase_;
    std::list<std::string> phaseList;

}; // AtomicModel

}; // DEVS

#endif
