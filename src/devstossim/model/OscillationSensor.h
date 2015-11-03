/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
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
