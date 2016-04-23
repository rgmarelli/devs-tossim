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

#ifndef __DEVS_MOTE_SERIAL__
#define __DEVS_MOTE_SERIAL__

#include <AtomicModel.h>
#include <string>

const std::string SERIAL_ON = "on";
const std::string SERIAL_OFF = "off";

const std::string SERIAL_TURNING_OFF = "turning_off";
const std::string SERIAL_TURNING_ON = "turning_on";

const std::string SERIAL_SENDING = "sending";

const std::string SERIAL_RECEIVING = "receiving";

class Serial : public DEVS::AtomicModel
{

public:
    Serial(const std::string name, unsigned int baud=57600) : AtomicModel(name), 
                                                              baud_(baud) 
                                  /* MIB520 Specification (Memsic) Speed: 57600 baud */
    {
        /* Data interchange with TinyOS */
        inputPorts().add( DEVS::Port(name,"SerialAMSend",1) );
        outputPorts().add( DEVS::Port(name,"SerialAMSendDone",1) );
        outputPorts().add( DEVS::Port(name,"SerialAMReceive",1) );

        /* Ports for turning ON and OFF with TinyOS */
        inputPorts().add( DEVS::Port(name,"SerialTurnOff",1) );
        inputPorts().add( DEVS::Port(name,"SerialTurnOn",1) );

        outputPorts().add( DEVS::Port(name,"SerialTurnOffDone",1) );
        outputPorts().add( DEVS::Port(name,"SerialTurnOnDone",1) );

        registerPhase(SERIAL_ON);
        registerPhase(SERIAL_OFF);

        registerPhase(SERIAL_TURNING_ON);
        registerPhase(SERIAL_TURNING_OFF);

        registerPhase(SERIAL_SENDING);

        registerPhase(SERIAL_RECEIVING);

        setPhase(SERIAL_OFF);
    }

    // Internal transition (delta Int)
    virtual void internalTransition() {      
        // Transceiver turning ON/OFF
        if( phaseIs(SERIAL_TURNING_ON) ) {
            setSigma(DEVS::Time::infinity());
            setPhase(SERIAL_ON);
            return;
        }

        if( phaseIs(SERIAL_TURNING_OFF) ) {
            setSigma(DEVS::Time::infinity());
            setPhase(SERIAL_OFF);
            return;
        }

        if( phaseIs(SERIAL_SENDING) ) {
            this->on_serial_sending();
            return;
        }

        if( phaseIs(SERIAL_RECEIVING) ) {
            setSigma(DEVS::Time::infinity());
            setPhase(SERIAL_ON);
            return;
        }
    }

    // External transition (delta Ext)
    virtual void externalTransition(DEVS::ExternalMessage* message) {

        if( phaseIs(SERIAL_OFF) && message->dstPort().nameIs("SerialTurnOn") ) {
            setSigma( 1*DEVS::Time::mili_to_nano_sec );
            setPhase(SERIAL_TURNING_ON);
            return;
        }

        if( phaseIs(SERIAL_ON) && message->dstPort().nameIs("SerialTurnOff") ) {
            setSigma( 1*DEVS::Time::mili_to_nano_sec );
            setPhase(SERIAL_TURNING_OFF);
            return;
        }

        if( phaseIs(SERIAL_ON) && message->dstPort().nameIs("SerialAMSend") ) {
            this->on_serial_am_send(message);
            setSigma(transfer_interval( message->size() ) );
            setPhase(SERIAL_SENDING);
            return;
        }

    }

    // Output function (Y)
    virtual DEVS::OutputMessage* outputFunction() {
        // Signal Turn ON/OFF to TinyOS
        if( phaseIs(SERIAL_TURNING_ON) ) {
            return new DEVS::OutputMessage(outputPorts().getPortByName("SerialTurnOnDone"));
        }
        if( phaseIs(SERIAL_TURNING_OFF) ) {
            return new DEVS::OutputMessage(outputPorts().getPortByName("SerialTurnOffDone"));
        }

        if( phaseIs(SERIAL_SENDING) ) {
            return new DEVS::OutputMessage(outputPorts().getPortByName("SerialAMSendDone"));
        }

        if( phaseIs(SERIAL_RECEIVING) ) {
            return this->on_serial_am_receive();
        }

        return NULL;

    }

protected:
    DEVS::Time transfer_interval( unsigned long data_size ) {
        double time = (data_size*8)/this->baud_;
        return DEVS::Time( DEVS::Time::to_nano_sec*time );
    }

    virtual void on_serial_sending() {
        setSigma(DEVS::Time::infinity());
        setPhase(SERIAL_ON);
    }

    virtual void on_serial_am_send(DEVS::ExternalMessage* message) {
        return;
    }

    virtual DEVS::OutputMessage* on_serial_am_receive() {
        return NULL;
    }

protected:
    unsigned int baud_;
};

#endif
