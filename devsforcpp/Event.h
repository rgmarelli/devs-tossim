/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_EVENT__
#define DEVS_CPP_EVENT__

#include "Simulator.h"
#include "Message.h"
#include "Time.h"

namespace DEVS {

class Event
{
public:
    bool external() const { return internal_==false; }
    bool internal() const { return internal_==true; }
    TIME TN() const { return TN_; }
    ExternalMessage* message() const { return message_; }
    Simulator* simulator() const { return simulator_; }

    static Event createExternalEvent(TIME TN,Simulator* simulator,ExternalMessage* message) {
        return Event(TN,simulator,message,false);
    }

    static Event createInternalEvent(TIME TN,Simulator* simulator) {
        return Event(TN,simulator,NULL,true);
    }
private:
    Event(TIME TN,Simulator* simulator,ExternalMessage* message, bool internal) :
                 TN_(TN),simulator_(simulator),message_(message), internal_(internal)
    {} 
    TIME TN_;
    Simulator* simulator_;
    ExternalMessage *message_;    
    bool internal_;
}; // Event

}; // DEVS

#endif


