/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
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
