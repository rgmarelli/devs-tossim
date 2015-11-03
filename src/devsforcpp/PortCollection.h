/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_PORT_COLLECTION__
#define DEVS_CPP_PORT_COLLECTION__

#include "Port.h"
#include "Log.h"
#include <list>

namespace DEVS {

class PortCollection
{
public:
    typedef std::list<Port> PortMap;
    typedef PortMap::const_iterator const_iterator;

    const_iterator begin() const { return ports_.begin(); }
    const_iterator end() const { return ports_.end(); }

    bool add(Port port) {
        if(!hasPort(port.name())) {
            ports_.push_back(port);
            return true;
        }

        Log::write(LOG_WARNING,"DEVS","[PortCollection] Port %s already exists",port.name().c_str());
        return false;
    }

    bool hasPort(const Port& port) const {
        return hasPort(port.name());
    }

    bool hasPort(const std::string name) const {
        return !getPortByName(name).empty();
    }

    Port getPort(const Port& port) const {
        return getPortByName(port.name());
    }

    Port getPortByName(std::string name) const {
        PortMap::const_iterator it = ports_.begin();     
        while(it != ports_.end()) {
            if(it->nameIs(name)) {
                return (*it);
            }
            it++;
        }   
        return Port(); // empty port
    }
    
private:
    PortMap ports_;
}; // Portcollection

} // DEVS

#endif

