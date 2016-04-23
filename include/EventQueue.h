/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_EVENT_QUEUE__
#define DEVS_CPP_EVENT_QUEUE__

#include "Event.h"
#include "Message.h"

#include "Log.h"

#include <list>

namespace DEVS {

class EventQueue
{
public:
    typedef std::list<Event> EventListType;
    void addExternalEvent(TIME t, Simulator* simulator, ExternalMessage* message) {
        Event event = Event::createExternalEvent(t,simulator,message);
        insertEvent(event);
    }
    void setInternalEvent(TIME t, Simulator* simulator) {
        removeInternalEvent(simulator);
        Event event = Event::createInternalEvent(t,simulator);
        insertEvent(event);
   }

   void removeSimulator( Simulator* simulator ) {
        events_.remove_if( RemoveIf( simulator ) );
   } 

   bool empty() const {
       return events_.empty();
   }

   unsigned int size() const {
       return events_.size();
   }

   void dump() const {
        EventListType::const_iterator it = events_.begin();	
        int count=0;
        while( it != events_.end() ) {
            const char* type = it->internal() ? "internal" : "external";
            Log::write(LOG_INFO, "DEVS", "[EventQueue] %s, TN=%s (%s), %s", it->simulator()->model_name().c_str(), 
                                                                            it->TN().to_string().c_str(),
                                                                            it->simulator()->nextTN().to_string().c_str(),
                                                                            type);
            it++;
        }
   }

   Event pop() {
       if(empty()) {
           // No deberia pasar
           Log::write(LOG_CRIT, "DEVS", "EventQueue is NULL (pop)");
           exit(0);
       }
       Event event = events_.front();
       events_.pop_front();
       return event;
   }

   Event front() {
       if(empty()) {
           // No deberia pasar
           Log::write(LOG_CRIT, "DEVS", "EventQueue is NULL (front)");
           exit(0);
       }
       return events_.front();
   }

private:
    class RemoveIf
    {
    public:
        RemoveIf( Simulator *simulator ) : simulator_(simulator)
        {}

        bool operator()(const Event &e ) {
            return e.simulator() == simulator_;
        }
    private:
        Simulator *simulator_;
    };

private:
    EventListType events_;

    void insertEvent(Event event) {
      
        EventListType::iterator it = events_.end();	
        int count=0;
        while( it != events_.begin() ) {
            it--;
            count++;
            if( !(it->TN() > event.TN()) ) {
                /* Inserto el evento despues del primer evento que tiene menor o igual TN.
                   De este modo eventos con el mismo TN son procesados en el orden de llegada */
                it++;
                events_.insert(it,event);
                return;
            }
        }
        events_.push_front(event); 
    }

    void removeInternalEvent(Simulator* simulator) {
        EventListType::iterator it = events_.begin();
        while( it != events_.end() ) {
            if(it->internal() && it->simulator()== simulator) {
                it = events_.erase(it);
            }
            else {
                it++;
            }
        }
    }
}; // EventQueue

}; // DEVS

#endif


