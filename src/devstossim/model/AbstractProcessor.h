/* 
 * DEVS-TOSSIM - a DEVS framework for simulation of TinyOS wireless sensor networks
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
        DEVS::Time t=0;
        if( 0 ) {
            /* This method cause a lot of data lost; it's quite difficult
               to meassure processor time... */
            unsigned long ATMega128_MIPS = 16; /* Atmega128 SPEC */
            unsigned long PC_MIPS = 100000; /* Aprox for an Intel core i7 */

            double factor = PC_MIPS/ATMega128_MIPS;
            t = elapsed*factor;
        }
        else {
            t = elapsed*100;
        }
        return t;
    }
};

#endif
