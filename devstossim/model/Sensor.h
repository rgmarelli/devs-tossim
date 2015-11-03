/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef __DEVS_MOTE_SENSOR__
#define __DEVS_MOTE_SENSOR__

#include <AtomicModel.h>

const std::string SENSOR_HOLD 		= "hold";
const std::string SENSOR_SENSING 	= "sensing";

class Sensor : public DEVS::AtomicModel
{
public:

    Sensor(const std::string name, 
           unsigned long time_to_read=10*DEVS::Time::mili_to_nano_sec ) : DEVS::AtomicModel(name), 
                                                                          time_to_read_( time_to_read )
    {
        outputPorts().add(DEVS::Port(name,"ReadDone",1));
        inputPorts().add(DEVS::Port(name,"Read",1));

        registerPhase(SENSOR_HOLD);
        registerPhase(SENSOR_SENSING);

        setPhase(SENSOR_HOLD);
    }

    virtual void internalTransition() {
        setSigma(DEVS::TIME::infinity());
        setPhase(SENSOR_HOLD);
    }

    // Funcion de transicion externa (deta Ext)
    virtual void externalTransition(DEVS::ExternalMessage* message) {
        if(phaseIs(SENSOR_HOLD) && message->dstPort().nameIs("Read")) {
            setSigma(this->time_to_read_); 
            setPhase(SENSOR_SENSING);
        }
    }

    // Funcion de salida (Y)
    virtual DEVS::OutputMessage* outputFunction() {
        DEVS::OutputMessage* o = new DEVS::OutputMessage(outputPorts().getPortByName("ReadDone"));
        int val = this->value();
        DEVS::Log::write(LOG_DEBUG,"DEVS::Sensor", "[%s] Read value=%d", name().c_str(),val);
        o->putContent<int>(val);
        return o;
    }

protected:
    virtual int value() = 0;

private:
    DEVS::Time time_to_read_;
};

#endif
