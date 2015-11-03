/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */

#ifndef DEVS_ABSTRACT_PROCESSOR__
#define DEVS_ABSTRACT_PROCESSOR__

#include <AtomicModel.h>

class AbstractProcessor : public DEVS::AtomicModel
{
public:
   AbstractProcessor( const std::string &pname ) : DEVS::AtomicModel(pname)
   {
        inputPorts().add(DEVS::Port(name(),"TimerFire0",1));
        outputPorts().add(DEVS::Port(name(),"TimerStart0",1));
        outputPorts().add(DEVS::Port(name(),"TimerStop0",1));

        inputPorts().add(DEVS::Port(name(),"TimerFire1",1));
        outputPorts().add(DEVS::Port(name(),"TimerStart1",1));
        outputPorts().add(DEVS::Port(name(),"TimerStop1",1));

        inputPorts().add(DEVS::Port(name(),"TimerFire2",1));
        outputPorts().add(DEVS::Port(name(),"TimerStart2",1));
        outputPorts().add(DEVS::Port(name(),"TimerStop2",1));

        inputPorts().add(DEVS::Port(name(),"TimerFire3",1));
        outputPorts().add(DEVS::Port(name(),"TimerStart3",1));
        outputPorts().add(DEVS::Port(name(),"TimerStop3",1));

        inputPorts().add(DEVS::Port(name(),"TimerFire4",1));
        outputPorts().add(DEVS::Port(name(),"TimerStart4",1));
        outputPorts().add(DEVS::Port(name(),"TimerStop4",1));

        inputPorts().add(DEVS::Port(name(),"SensorReadDone",1));
        outputPorts().add(DEVS::Port(name(),"SensorRead",1));

        outputPorts().add( DEVS::Port(name(),"AMTurnOn",1) );
        inputPorts().add( DEVS::Port(name(),"AMTurnOnDone",1) );

        outputPorts().add( DEVS::Port(name(),"AMTurnOff",1) );
        inputPorts().add( DEVS::Port(name(),"AMTurnOffDone",1) );

        outputPorts().add( DEVS::Port(name(),"AMSend",1) );
        inputPorts().add( DEVS::Port(name(),"AMSendDone",1) );

        inputPorts().add( DEVS::Port(name(),"AMReceive",1) );

        outputPorts().add( DEVS::Port(name(),"SerialAMTurnOn",1) );
        inputPorts().add( DEVS::Port(name(),"SerialAMTurnOnDone",1) );

        outputPorts().add( DEVS::Port(name(),"SerialAMTurnOff",1) );
        inputPorts().add( DEVS::Port(name(),"SerialAMTurnOffDone",1) );

        outputPorts().add( DEVS::Port(name(),"SerialAMSend",1) );
        inputPorts().add( DEVS::Port(name(),"SerialAMSendDone",1) );

        inputPorts().add( DEVS::Port(name(),"SerialAMReceive",1) );
    
    }

    virtual void internalTransition() = 0;
    virtual void externalTransition(DEVS::ExternalMessage* message) = 0;
    virtual DEVS::OutputMessage* outputFunction() = 0;

protected:
    DEVS::Time scale_time( DEVS::Time &elapsed) {
        /* This is a simple approximation that can be used to calculate sigma for the phase.
         *           
         */
        unsigned long ATMega128_MIPS = 16; /* Atmega128 SPEC */
        unsigned long PC_MIPS = 100000; /* Aprox for an Intel core i7 */

        double factor = PC_MIPS/ATMega128_MIPS; 

        return elapsed*factor;
    }
};

#endif
