/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */

module McuSleepC {
  provides {
    interface McuSleep;
    interface McuPowerState;
  }
}
implementation {
  async command void McuSleep.sleep() {
      putOutputMessage("Sleep","Sleep",NULL,0);
  }

  async command void McuPowerState.update() {
  }
}
