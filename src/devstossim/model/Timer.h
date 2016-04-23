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

    // Internal transition (delta Int)
    virtual void internalTransition() {
        if(isOneShot()) {
            setSigma(DEVS::TIME::infinity());
            setPhase(TIMER_DISABLED);
        }
    }

    // External transition (delta Ext)
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

    // Output function (Y)
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
