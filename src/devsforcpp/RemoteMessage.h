/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_REMOTE_MESSAGE__
#define DEVS_CPP_REMOTE_MESSAGE__

#include "Message.h"

namespace DEVS {

class RemoteMessage
{
public:

    static const unsigned int MAX_RAW_DATA_BUFFER = 1024;

    static const char TYPE_CONNECTION_REQUEST           = 1;
    static const char TYPE_CONNECTION_REPLY             = 2;  
    static const char TYPE_EXTERNAL_TRANSITION_REQUEST  = 3;
    static const char TYPE_EXTERNAL_TRANSITION_REPLY    = 4;
    static const char TYPE_INTERNAL_TRANSITION_REQUEST  = 5;
    static const char TYPE_INTERNAL_TRANSITION_REPLY    = 6;

    static const char TYPE_DISCONNECT_REQUEST           = 7;
    static const char TYPE_DISCONNECT_REPLY             = 8;

    RemoteMessage(const void* raw_data, unsigned int raw_size) : raw_size_(raw_size) {
        memcpy( &message_, raw_data, raw_size );
    }

    RemoteMessage(char type, TIME tn, const char* message, unsigned int message_size) {
        if( message != NULL && message_size > 0 ) {
            memcpy( message_.payload, message, message_size );
        }
        this->write_header( type, tn, message_size );
    }

    RemoteMessage(char type, TIME tn, const Message* m) {
        unsigned int message_size = 0;
        if( m != NULL ) {
            message_size = m->serialize( message_.payload );
        }
        this->write_header( type, tn, message_size ); 
    }

    virtual ~RemoteMessage() {
    }

    template <class T> T* getMessage() {
        if( raw_size_ > header_size() ) {
            unsigned int size = this->payload_size();
            if(size > 0) {
                return new T( message_.payload, size );
            }
        }
        return NULL;
    }

    char type() { return message_.header.type; }

    TIME nextTN() {
        return TIME( message_.header.sec, message_.header.nsec );
    }

    void* raw_data() { return &message_; };
    unsigned int raw_size() { return raw_size_; };    

    unsigned int header_size() { return sizeof(Header); }

    char* payload() { return message_.payload; }
    unsigned int payload_size() { return message_.header.payload_size; }

    static unsigned int max_message_size() { return sizeof(Message); }

private:

    struct Header {
        char type;
        time_t sec;
        long nsec;
        unsigned int payload_size;
    };

    struct Message {
        Header header;
        char payload[MAX_RAW_DATA_BUFFER]; 
    };

    void write_header(char type, TIME tn,unsigned int message_size) {
        message_.header.type = type;
        message_.header.sec = tn.sec();
        message_.header.nsec = tn.nsec();
        message_.header.payload_size = message_size;

        this->raw_size_ = header_size() + message_size;
    }

    unsigned int raw_size_;
    Message message_;

}; // RemoteMessage

}; // DEVS

#endif


