/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_MOTE_DATASENSOR__
#define DEVS_MOTE_DATASENSOR__

#include "Sensor.h"
#include <DataCollector.h>

class DataSensor : public Sensor
{
public:

    DataSensor(const std::string name, DEVS::DataCollector<>* collector) : Sensor(name), last_value_(0), collector_(collector)
    {
    }

protected:
    virtual int value() {
        DEVS::Stream & s = collector_->collected_data();
        int size=0;
        while( size < s.size() ) {
            if( s[size] == '\n' ) {
                if( size > 0 ) {
                    unsigned char recvbuffer[512];
                    memset( recvbuffer, 0, sizeof(recvbuffer) );
                    unsigned int total_read = collector_->collected_data().read( recvbuffer, size+1);
                    if( total_read != size+1 ) {
                        DEVS::Log::write(LOG_ERR, "DEVS::DataSensor", "[%s] Error reading sensing information from data client", name().c_str());
                    }
                    if( total_read == 2 ) {
                        collector_->disconnect();
                    }
                    else {
                        this->last_value_ = atoi( (char*)recvbuffer );
                    }
                    break;
                }
            }
            size++;
        }

        return this->last_value_;
    }
private:
    unsigned int last_value_;
    DEVS::DataCollector<>* collector_;
};

#endif //DEVS_MOTE_DATASENSOR__
