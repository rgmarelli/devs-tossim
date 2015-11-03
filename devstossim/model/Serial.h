/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
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
        /* Intercambio de paquetes con TinyOS. O mejor dicho: con la capa MAC */
        inputPorts().add( DEVS::Port(name,"SerialAMSend",1) );
        outputPorts().add( DEVS::Port(name,"SerialAMSendDone",1) );
        outputPorts().add( DEVS::Port(name,"SerialAMReceive",1) );

        /* Para poder apagarlo y prenderlo con TinyOS */
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

    // Funcion de transicion externa (delta Ext)
    virtual void externalTransition(DEVS::ExternalMessage* message) {

        if( phaseIs(SERIAL_OFF) && message->dstPort().nameIs("SerialTurnOn") ) {
            /* Esta transicion no debe ser inmediata.
               TinyOS ordena start mediante split control y debe monitorear una interrupcion
               para generar el evento de startDone */
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

    // Funcion de salida (Y)
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
