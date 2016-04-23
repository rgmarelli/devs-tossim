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

#ifndef DEVS_MOTE__
#define DEVS_MOTE__

#include <CoupledCompositeModel.h>

#include "Timer.h"
#include "Transceiver.h"
#include "Sensor.h"
#include "OscillationSensor.h"
#include "Timer.h"
#include "Serial.h"

class Mote : public DEVS::CoupledCompositeModel
{
public:
    Mote(const std::string name, DEVS::Model *processor, float tranceiver_duty_cycle, Sensor* sensor=NULL, Serial *serial=NULL) 
                                                                                                        : DEVS::CoupledCompositeModel(name)
    {
        // Add input/output ports of the Mote coupled model
        outputPorts().add( DEVS::Port(name,"RadioTransmit") );
        inputPorts().add( DEVS::Port(name,"RadioReceive") );

        // Couple ports for all the component models
        this->addProcessor(processor);
        this->addTimer( processor, new Timer(name + ".Timer0"), "0" );
        this->addTimer( processor, new Timer(name + ".Timer1"), "1" );
        this->addTimer( processor, new Timer(name + ".Timer2"), "2" );
        this->addTimer( processor, new Timer(name + ".Timer3"), "3" ); 
        this->addTimer( processor, new Timer(name + ".Timer4"), "4" ); 

        if( sensor == NULL ) {
            sensor = new OscillationSensor(name + ".Sensor", true /* randomize sensor value */);
        }
        this->addSensor( processor, sensor );

        this->addTransceiver( processor, new Transceiver(name + ".Transceiver", tranceiver_duty_cycle) );

        if( serial == NULL ) {
            serial = new Serial(name + ".Serial");
        }
        this->addSerial( processor, serial );

        addCoupling(processor->outputPorts().getPortByName("Sleep"),processor->inputPorts().getPortByName("SleepIn"));
    }

private:
    void addProcessor( DEVS::Model* processor ) {
        add(processor);
    }

    void addTransceiver( DEVS::Model* processor, Transceiver *transceiver ) {
        add(transceiver);

        // Couple the transceiver to the processor
        addCoupling(processor->outputPorts().getPortByName("AMTurnOn"),transceiver->inputPorts().getPortByName("TurnOn"));
        addCoupling(transceiver->outputPorts().getPortByName("TurnOnDone"),processor->inputPorts().getPortByName("AMTurnOnDone"));        

        addCoupling(processor->outputPorts().getPortByName("AMTurnOff"),transceiver->inputPorts().getPortByName("TurnOff"));
        addCoupling(transceiver->outputPorts().getPortByName("TurnOffDone"),processor->inputPorts().getPortByName("AMTurnOffDone"));        

        addCoupling(processor->outputPorts().getPortByName("AMSend"),transceiver->inputPorts().getPortByName("AMSend"));
        addCoupling(transceiver->outputPorts().getPortByName("AMSendDone"),processor->inputPorts().getPortByName("AMSendDone"));

        addCoupling(transceiver->outputPorts().getPortByName("AMReceive"),processor->inputPorts().getPortByName("AMReceive"));


        // Couple the transceiver to the input/output ports of the mote model
        addCoupling( transceiver->outputPorts().getPortByName("RadioTransmit"), this->outputPorts().getPortByName("RadioTransmit") );
        addCoupling( this->inputPorts().getPortByName("RadioReceive"), transceiver->inputPorts().getPortByName("RadioReceive") );
    }

    void addSerial( DEVS::Model* processor, Serial *serial ) {
        add(serial);

        // Couple serial to the processor
        addCoupling(processor->outputPorts().getPortByName("SerialAMTurnOn"),serial->inputPorts().getPortByName("SerialTurnOn"));
        addCoupling(serial->outputPorts().getPortByName("SerialTurnOnDone"),processor->inputPorts().getPortByName("SerialAMTurnOnDone"));        

        addCoupling(processor->outputPorts().getPortByName("SerialAMTurnOff"),serial->inputPorts().getPortByName("SerialTurnOff"));
        addCoupling(serial->outputPorts().getPortByName("SerialTurnOffDone"),processor->inputPorts().getPortByName("SerialAMTurnOffDone"));        

        addCoupling(processor->outputPorts().getPortByName("SerialAMSend"),serial->inputPorts().getPortByName("SerialAMSend"));
        addCoupling(serial->outputPorts().getPortByName("SerialAMSendDone"),processor->inputPorts().getPortByName("SerialAMSendDone"));

        addCoupling(serial->outputPorts().getPortByName("SerialAMReceive"),processor->inputPorts().getPortByName("SerialAMReceive"));
    }

    void addSensor( DEVS::Model* processor, Sensor *sensor ) {
        add(sensor);
        addCoupling(processor->outputPorts().getPortByName("SensorRead"),sensor->inputPorts().getPortByName("Read"));
        addCoupling(sensor->outputPorts().getPortByName("ReadDone"),processor->inputPorts().getPortByName("SensorReadDone"));
    }

    void addTimer( DEVS::Model* processor, Timer *timer, std::string timer_id ) {
        add(timer);

        addCoupling(processor->outputPorts().getPortByName("TimerStop"+timer_id),timer->inputPorts().getPortByName("TimerStop"));
        addCoupling(processor->outputPorts().getPortByName("TimerStart"+timer_id),timer->inputPorts().getPortByName("TimerStart"));
        addCoupling(timer->outputPorts().getPortByName("TimerFire"),processor->inputPorts().getPortByName("TimerFire"+timer_id));
    }

};

#endif
