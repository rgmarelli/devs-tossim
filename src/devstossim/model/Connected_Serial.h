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

#ifndef DEVS_MOTE_CONNECTED_SERIAL__
#define DEVS_MOTE_CONNECTED_SERIAL__

#include "Serial.h"
#include <DataCollector.h>
#include "Serial_Handshake.h"

class Connected_Serial : public Serial
{
public:
    typedef DEVS::DataCollector<Serial_Handshake> Collector_Type;

    Connected_Serial(const std::string name, Collector_Type* collector) : Serial(name), collector_(collector)
    {}

protected:
    virtual void on_serial_sending() {
        if( collector_->collected_data().size() > 0 ) {
            DEVS::Stream & s = collector_->collected_data();
            unsigned int size = s[0];
            setSigma( this->transfer_interval( size ) );
            setPhase(SERIAL_RECEIVING);
        }
        else {
            setSigma(DEVS::Time::infinity());
            setPhase(SERIAL_ON);
        }
    }

    virtual void on_serial_am_send(DEVS::ExternalMessage* message) {
        unsigned char buffer[message->size()+2];
        buffer[0] = message->size()+1;
        buffer[1] = 0;
        memcpy(&buffer[2], message->content(), message->size());

        if( collector_->send( buffer, sizeof(buffer) ) == -1 ) {
            DEVS::Log::write(LOG_ERR, "DEVS::ConnectedSerial", "[%s] Error sending message", name().c_str());
        }
    }

    virtual DEVS::OutputMessage* on_serial_am_receive() {
        DEVS::Stream & s = collector_->collected_data();
        unsigned int l = s[0];
        if( l < s.size() ) {
                unsigned char recvbuffer[512];
                memset( recvbuffer, 0, sizeof(recvbuffer) );
                unsigned int total_read = collector_->collected_data().read( recvbuffer, l+1);
                std::cout << "Read: " << total_read << " bytes" << std::endl;
                if( total_read != l+1 ) {
                        std::cout << "Recv err" << std::endl;
                        return NULL;
                }

                DEVS::OutputMessage* message = new DEVS::OutputMessage(outputPorts().getPortByName("SerialAMReceive"));
                message->putContent( &recvbuffer[2], total_read-2 );
                return message;
        }
        return NULL;
    }

private:
    Collector_Type* collector_;
};

#endif //__DEVS_MOTE_CONNECTED_SERIAL__
