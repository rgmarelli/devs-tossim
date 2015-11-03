/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_MOTE_TIMER__
#define DEVS_MOTE_TIMER__

#include <AtomicModel.h>
#include <platforms/devs/TimerMsg.h>
#include <stdint.h>

const std::string TIMER_ENABLED = "enabled";
const std::string TIMER_DISABLED = "disabled";

class Timer : public DEVS::AtomicModel
{
public:
    Timer(const std::string name) : DEVS::AtomicModel(name), one_shot_(false)
    {
        outputPorts().add(DEVS::Port(name,"TimerFire",1));
        inputPorts().add(DEVS::Port(name,"TimerStart",1));
        inputPorts().add(DEVS::Port(name,"TimerStop",1));

        registerPhase(TIMER_ENABLED);
        registerPhase(TIMER_DISABLED);

        setPhase(TIMER_DISABLED);
    }

    virtual void internalTransition() {
        if(isOneShot()) {
            setSigma(DEVS::TIME::infinity());
            setPhase(TIMER_DISABLED);
        }
    }

    // Funcion de transicion externa (delta Ext)
    virtual void externalTransition(DEVS::ExternalMessage* message) {


        DEVS::Log::write(LOG_DEBUG,"DEVS", "%s externalTransition %s",name().c_str(), message->dstPort().name().c_str());

        if( phaseIs(TIMER_DISABLED) && message->dstPort().nameIs("TimerStart") ) {

            TimerMsg *m = (TimerMsg*)message->content();

            setSigma(m->dt*DEVS::Time::mili_to_nano_sec);
            one_shot_= (m->one_shot == 1);
            DEVS::Log::write(LOG_DEBUG,"DEVS", "%s externalTransition %d",name().c_str(), m->dt);
            setPhase(TIMER_ENABLED);
            return;
        }
        if( phaseIs(TIMER_ENABLED) && message->dstPort().name() == "TimerStop") {
            setSigma(DEVS::TIME::infinity());
            setPhase(TIMER_DISABLED);
            return;
        }
    }

    // Funcion de salida (Y)
    virtual DEVS::OutputMessage* outputFunction() {
        if( phaseIs(TIMER_ENABLED) ) {
            return new DEVS::OutputMessage(outputPorts().getPortByName("TimerFire"));
        }
        return NULL;
    }

    bool isOneShot() {
        return one_shot_;
    }

private:
    bool one_shot_;
};

#endif
