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
