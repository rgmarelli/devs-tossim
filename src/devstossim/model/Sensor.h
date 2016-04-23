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

    // Internal transition (delta Int)
    virtual void internalTransition() {
        setSigma(DEVS::TIME::infinity());
        setPhase(SENSOR_HOLD);
    }

    // External transition (delta Ext)
    virtual void externalTransition(DEVS::ExternalMessage* message) {
        if(phaseIs(SENSOR_HOLD) && message->dstPort().nameIs("Read")) {
            setSigma(this->time_to_read_); 
            setPhase(SENSOR_SENSING);
        }
    }

    // Output function (Y)
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
