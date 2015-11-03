/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_SIMULATOR__
#define DEVS_CPP_SIMULATOR__

#include "OutputMessage.h"
#include "ExternalMessage.h"
#include "Time.h"

namespace DEVS {

/*
 * Simulator es la interfaz basica de todo simulador y el principio que permite
   abstraer del lugar donde se ejecuta el modelo.
 */

class Simulator
{
public:
    Simulator() : error_(false)
    {}

    virtual ~Simulator() 
    {}

    virtual void externalTransition(TIME t, ExternalMessage* message)=0;
    virtual OutputMessage* internalTransition(TIME t)=0;
    virtual TIME nextTN() const =0;

    virtual std::string model_name() const = 0;

    bool error() { return error_; }

protected:
    void error( bool e ) { error_ = e; }

private:
    bool error_;

}; // Simulator

}; // DEVS

#endif


