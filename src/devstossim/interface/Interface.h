/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * Interfaz de DEVS-TOSSIM con TinyOS.
 */

#ifndef DEVS_INTERFACE__
#define DEVS_INTERFACE__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

/* Devuelve una representación del tiempo. Se utiliza en TinyOS en mensajes de debug.
 * Estrictamente no es necesario implementarla (podria devolver string vacio).
 */
char* sim_time_string();

uint32_t sim_tn();

typedef void (*ExternalMessageHandler)(const void* port_name, const void* content, unsigned int content_size);
typedef int (*RunTaskFunctionHandle)();

/* Registra la función de TinyOS encargada de ejecutar la siguiente tarea.
 * Se invoca cuando el modelo realiza una transición interna.
 */
void registerRunTaskFunction(RunTaskFunctionHandle runTraskFunction);

/* Para procesar eventos externos. 
 * Esta función se utilza para mapear un evento DEVS a un evento de un componene de TinyOS.
 * Para tal fin, se utiliza el nombre del puerto.
 */
void registerExternalEvent(char* port_name,ExternalMessageHandler handle);


/* Enviar un mensaje de salida al simulador DEVS. 
 * state_name es el estado en el que permanecera el modelo hasta que el mensaje sea enviado.
 */
void putOutputMessage(const char* state_name, const char* port_name, 
                      const void* content, unsigned int content_size);

/* Escribir/leer indice de puerto DEVS en un string */
char* makePortName(const char* name,unsigned int num);
unsigned int getIndexFromPortName(const char* port_name);

/* Funcion de loggin utilizada por TinyOS */
void debug(unsigned int id, char* string, const char* format, ...);

/* Esta funcion es pasada la compilador mediante el parametro: -fnesc-nido-motenumber. */
unsigned long sim_node();

unsigned long set_sim_node(unsigned long id, uint32_t tn);

#ifdef __cplusplus
}
#endif

#endif
