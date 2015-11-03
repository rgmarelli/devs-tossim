/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * Clase TinyOSProcessor.
 */

#include "TinyOSProcessor.h"

DEVS::OutputMessage* TinyOSProcessor::outputFunction() {
    if( phaseIs(PROCESSOR_WORKING) ) return NULL; 

    if(outputStates_.size() > 0) {
        OutputState outputState = outputStates_.front();
        outputStates_.pop_front();
        return outputState.message();
    }
    return NULL;
}

void TinyOSProcessor::putOutputMessage(const char* state_name, const char* port_name, 
                                    const void* content, unsigned int content_size)
{
    if(!outputPorts().hasPort(port_name)) {

        DEVS::Log::write(LOG_ERR, name().c_str(),  "TinyOSProcessor::putOutputMessage no outputPort: %s", port_name);
        exit(0);

    }
    DEVS::OutputMessage* message = new DEVS::OutputMessage(outputPorts().getPortByName(port_name));
    message->putContent(content,content_size);
    outputStates_.push_back(OutputState(state_name,message));
}

void TinyOSProcessor::registerExternalEvent(const char* portname,ExternalMessageHandler handle)
{
    if(!inputPorts().hasPort(portname)) {
        DEVS::Log::write(LOG_ERR,name().c_str(), "TinyOSProcessor::registerExternalEvent no inputPort: %s", portname);
        exit(0);
    }
    externalMessageHandlers_[portname] = handle;
}

void TinyOSProcessor::registerRunTaskFunction(RunTaskFunctionHandle runTaskFunction) {
    runTaskFunction_ = runTaskFunction;
}

void TinyOSProcessor::doExternalTransition(DEVS::ExternalMessage* message) {

    if( message->dstPort().nameIs("SleepIn") ) {
        if( outputStates_.size() > 0 ) {
            setSigma(0);
        }
        else {           
            setSigma( DEVS::TIME::infinity() );
            if( phase() != PROCESSOR_SLEEPING ) setPhase( PROCESSOR_SLEEPING );
        }
        return;
    }

    setPhase(PROCESSOR_EXECUTING_EVENT);
    ExternalMessageHandler handler = getExternalMessageHandler(message->dstPort().name());
    if(handler != NULL) {
        DEVS::Time t1 = DEVS::Time::cpu_time();
        handler(message->dstPort().name().c_str(),message->content(),message->size());
        DEVS::Time elapsed = DEVS::Time::cpu_time() - t1;
        setSigma(this->scale_time(elapsed));
    }
    else {

    }
}

void TinyOSProcessor::doInternalTransition() {   

    if( phaseIs(PROCESSOR_EXECUTING_EVENT) ) {
        setPhase(PROCESSOR_WORKING); 
        return;
    }

    if( phaseIs(PROCESSOR_POST_WORK) ) {
        if( outputStates_.size()== 0 ) {
            setPhase(PROCESSOR_WORKING);
        }
        return;
    }
    if ( phaseIs(PROCESSOR_WORKING) ) {
        if( outputStates_.size() > 0 ) {
            setPhase(PROCESSOR_POST_WORK);
            setSigma(0);
        }
        else {
            DEVS::Time t1 = DEVS::Time::cpu_time();
            unsigned int id = runTaskFunction();
            if(id == 0) {
                // En la implementación actual, esto sucede cuando el componente MCUSleep fue invocado.
                if( outputStates_.size() > 0 ) {
                    setPhase(PROCESSOR_POST_WORK);
                }
                setSigma(0);
                return;
            }
            DEVS::Time elapsed = DEVS::Time::cpu_time() - t1;
            setSigma(this->scale_time(elapsed));
        }   
        return;
    }
}   

