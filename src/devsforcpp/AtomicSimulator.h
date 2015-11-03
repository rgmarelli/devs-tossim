/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
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


