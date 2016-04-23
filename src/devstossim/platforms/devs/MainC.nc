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
 * This source code file has use MainC.nc fron TOSSIM (Philip Levis) 
 * as a reference implementation.
 *
 * DEVS-TOSSIM interface Boot implementation.
 */


#include "hardware.h"

configuration MainC {
  provides interface Boot;
  uses interface Init as SoftwareInit;
}
implementation {
  components PlatformC, SimMainP, TinySchedulerC, McuSleepC;

  SimMainP.Scheduler -> TinySchedulerC;
  SimMainP.PlatformInit -> PlatformC;
  SimMainP.McuSleep -> McuSleepC;

  // Export the SoftwareInit and Booted for applications
  SoftwareInit = SimMainP.SoftwareInit;
  Boot = SimMainP;

}

