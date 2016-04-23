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
#ifndef DEVS_CPP_ATOMIC_SIMULATOR__
#define DEVS_CPP_ATOMIC_SIMULATOR__

#include "AtomicModel.h"
#include "Simulator.h"
#include "OutputMessage.h"
#include "ExternalMessage.h"

namespace DEVS {

class AtomicSimulator : public Simulator
{
public:
    AtomicSimulator(AtomicModel *model, TIME now=DEVS::TIME::now()) : model_(model)
    {
        setNextTN(now);
    }
    
    virtual ~AtomicSimulator() 
    {}

    OutputMessage* internalTransition(TIME t) {
        if(t == nextTN()) {
            OutputMessage* outputMessage = model_->outputFunction();
            model_->internalTransition(t);
            setNextTN(t);
            return outputMessage;
        }
        setNextTN(t);
        return NULL;
    }
 
    void externalTransition(TIME t, ExternalMessage* message) {
        model_->externalTransition(t,message);  
        setNextTN(t);
        delete(message);
    }

    virtual TIME nextTN() const {
        return nextTN_;
    }

    virtual Model* model() const {
        return model_;
    }

    virtual std::string model_name() const {
        return model_->name();
    }

private:
    AtomicModel *model_;

    TIME nextTN_;

    void setNextTN(TIME t) {
        if( model_->timeAdvanceFunction() == DEVS::TIME::infinity() ) {
            nextTN_ = DEVS::TIME::infinity();
        }
        else {
            nextTN_ = t + model_->timeAdvanceFunction();
        }
    }
}; // AtomicSimulator

}; // DEVS

#endif


