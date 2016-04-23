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

#ifndef __DEVS_MOTE_OSCILLATIONSENSOR__
#define __DEVS_MOTE_OSCILLATIONSENSOR__

#include "Sensor.h"

class OscillationSensor : public Sensor
{
public:

    OscillationSensor(const std::string name, bool randomize=true, int value=0) : Sensor(name)
    {
        srand( time(NULL) );
        if( randomize ) {
            this->base_value = 100*(rand() % (500-50) + 50);
        }
        else {
            this->base_value = value;
        }
    }

protected:
    virtual int value() {
        int variation = (rand() % 1000);
        return this->base_value + variation;
    }
private:
    unsigned int base_value;
};

#endif //OSCILLATIONSENSOR
