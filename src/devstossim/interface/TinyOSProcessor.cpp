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
 *
 * ---
 * TinyOSProcessor implementation
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
                // This happens when MCUSleep was invoked
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

