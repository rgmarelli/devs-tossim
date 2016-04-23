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
 * DEVS-TOSSIM Timer implementation.
 */

#include "TimerMsg.h"
#define DEVSTIMERCOUNT 5

module HilTimerMilliC
{
  provides interface Init;
  provides interface Timer<TMilli> as TimerMilli[ uint8_t num ];
  provides interface LocalTime<TMilli>;
}
implementation
{
  
  
  TimerMsg info[DEVSTIMERCOUNT];
  bool is_running[DEVSTIMERCOUNT];  
  uint32_t timer_t0[DEVSTIMERCOUNT];

  void externalTimerFire(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {
      unsigned int num=getIndexFromPortName(port_name);
      if( info[num].one_shot == 1 ) {
          is_running[num] = FALSE;
      }
      else {
          timer_t0[num] = sim_tn();
      }
      signal TimerMilli.fired[num]();
  }

  command error_t Init.init() {
    int i=0;

    registerExternalEvent(makePortName("TimerFire",0),&externalTimerFire);
    registerExternalEvent(makePortName("TimerFire",1),&externalTimerFire);
    registerExternalEvent(makePortName("TimerFire",2),&externalTimerFire);
    registerExternalEvent(makePortName("TimerFire",3),&externalTimerFire);
    registerExternalEvent(makePortName("TimerFire",4),&externalTimerFire);

    for( i=0;i<DEVSTIMERCOUNT;i++) {
        info[i].dt=0;
        info[i].one_shot=0;
        is_running[i] = FALSE;
    }
    return SUCCESS;
  }

  command void TimerMilli.startPeriodic[ uint8_t num ]( uint32_t dt ) {
      info[num].dt = dt;
      info[num].one_shot = 0;
      is_running[num]=TRUE;
      timer_t0[num] = sim_tn();
      putOutputMessage("Start_Timer",makePortName("TimerStart",num),&info[num],sizeof(info));
  }
  command void TimerMilli.startOneShot[ uint8_t num ]( uint32_t dt ) {
      info[num].dt = dt;
      info[num].one_shot = 1;
      is_running[num]=TRUE;
      timer_t0[num] = sim_tn();
      putOutputMessage("Start_Timer",makePortName("TimerStart",num),&info[num],sizeof(TimerMsg));
  }

  command void TimerMilli.stop[ uint8_t num ]() {
      is_running[num]=FALSE;
      putOutputMessage("Stop_Timer",makePortName("TimerStop",num),NULL,0);
  }

  command bool TimerMilli.isRunning[ uint8_t num ]() {
    // return TRUE if the timer has been started and has not been cancelled 
    // and has not fired for the case of one-shot timers. 
    // Once a periodic timer is started, isRunning will return TRUE until it is cancelled.
    return is_running[num];
  }

  command bool TimerMilli.isOneShot[ uint8_t num ]() {
    return info[num].one_shot==1;
  }

  command void TimerMilli.startPeriodicAt[ uint8_t num ]( uint32_t t0, uint32_t dt ) {
    // As with alarms, the time t0 is always assumed to be in the past
  }

  command void TimerMilli.startOneShotAt[ uint8_t num ]( uint32_t t0, uint32_t dt ) {
  }

  /*
   * NOW me lo informa DEVS. Va a haber que pasar este parametro a las funciones de transicion externa e interna.
   * ¿Cual es la diferencia con Localtime.get()?
   */

  command uint32_t TimerMilli.getNow[ uint8_t num ]() {
    // return the current time in the precision and width of the timer.
    return sim_tn();
  }

  command uint32_t TimerMilli.gett0[ uint8_t num ]() {
    // return the time anchor for the previously started timer or the time of the previous event for periodic timers.
   return timer_t0[num];
  }

  command uint32_t TimerMilli.getdt[ uint8_t num ]() {
    // return the delay or period for the previously started timer. 
    return info[num].dt;
  }

  async command uint32_t LocalTime.get() {
    // return the current time.
    // es trivial. Deberia proporcionalo devs (current TN)
      printf("TimerMilli.LocalTime \n");
    return 0;
  }

  default event void TimerMilli.fired[uint8_t num]() {}


}
