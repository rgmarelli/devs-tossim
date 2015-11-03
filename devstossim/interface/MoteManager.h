/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * Clase MoteManager.
 * Sirve de interface entre TinyOS y los modelos DEVS. 
 * Se utiliza para localizar el modelo DEVS de procesador a partir del ID de nodo de TinyOS.
 */

#ifndef __MOTE_MANAGER__
#define __MOTE_MANAGER__

#include "TinyOSProcessor.h"
#include <map>

class MoteManager
{
public:
    static MoteManager* instance() { 
        if(instance_==NULL) {
            instance_ = new MoteManager();
        }
        return instance_; 
    }

    TinyOSProcessor* add(TinyOSProcessor* processor) {
        models_[processor->mote_id()] = processor;
        return processor;
    }

    TinyOSProcessor* get(unsigned long index) {
        std::map<unsigned long, TinyOSProcessor*>::iterator it = models_.find( index );
        if( it == models_.end() ) {
            exit(0);
        }

        if(it->second==NULL) {
            exit(0);
        }
        return it->second;
    }

private:
    static MoteManager* instance_;    
    std::map<unsigned long, TinyOSProcessor*> models_;

    MoteManager() {
    }
};


#endif

