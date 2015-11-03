/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * This source code file has use MainC.nc fron TOSSIM (Philip Levis) 
 * as a reference implementation.
 *
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

