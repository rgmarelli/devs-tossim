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
 * DEVS-TOSSIM interface with TinyOS
 */


#include "Interface.h"
#include "MoteManager.h"
#include "TinyOSProcessor.h"
#include <Log.h>
#include <sstream>

static uint32_t current_tn = 0;

void registerRunTaskFunction(RunTaskFunctionHandle runTraskFunction)
{
    TinyOSProcessor* model = MoteManager::instance()->get(sim_node());
    model->registerRunTaskFunction(runTraskFunction);
}

void registerExternalEvent(char* port_name,ExternalMessageHandler handle)
{
    TinyOSProcessor* model = MoteManager::instance()->get(sim_node());
    model->registerExternalEvent(port_name,handle);
}

void putOutputMessage(const char* state_name, const char* port_name, 
                      const void* content, unsigned int content_size)
{
    TinyOSProcessor* model = MoteManager::instance()->get(sim_node());
    model->putOutputMessage(state_name,port_name,content,content_size);
}

char* makePortName(const char* name,unsigned int num)
{
    #define MAX_PORT_NAME_SIZE 255
    #define PORT_NAME_SIZE (MAX_PORT_NAME_SIZE+10)
    static char port_name[PORT_NAME_SIZE];
    snprintf(port_name,PORT_NAME_SIZE,"%s%u",name,num);
    return port_name;
}

unsigned int getIndexFromPortName(const char* port_name)
{
    unsigned int r=-1;
    sscanf(port_name,"%*[^0-9]%u",&r);
    return r;
}

unsigned int TOS_NODE_ID=0;

unsigned long sim_node()
{
    return TOS_NODE_ID;
}

unsigned long set_sim_node(unsigned long id, uint32_t tn)
{
    TOS_NODE_ID = id;
    current_tn = tn;
}

char* sim_time_string() 
{
    static char time_str[255];
    strcpy( time_str, DEVS::Time::now().to_string().c_str() ); 
    return time_str;
}

uint32_t sim_tn() 
{
	return current_tn;
}

void debug(unsigned int id, char* string, const char* format, ...) {
    char message[1024];
    va_list args;
    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);
    if(message[strlen(message)-1] == '\n') {
        // Quitamos el salto de linea, y dejamos que el logger se encargue de esto
        message[strlen(message)-1] = '\0'; 
    }

    std::stringstream info;
    info << "TINYOS " << sim_node() << ":" << string;

    DEVS::Log::write(0, info.str().c_str(), message); 
}


