/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 *
 * DEVS-TOSSIM sensor implementation.
 */

module DevsSensorC
{
    provides interface Init;
    provides interface Read<uint16_t>;
}

implementation
{

    void readDone(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {
        unsigned int val=*((unsigned int*)content);
        signal Read.readDone(SUCCESS, val);
    }

    command error_t Init.init() {
        registerExternalEvent("SensorReadDone",&readDone);
        return SUCCESS;
    }

    command error_t Read.read() {
        putOutputMessage("Sensor_Read","SensorRead",NULL,0);
        return SUCCESS;
    }
}
