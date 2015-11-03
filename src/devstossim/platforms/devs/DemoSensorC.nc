/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 *
 * DemoSensor for the devs platform.
 */

generic configuration DemoSensorC(){
  provides interface Read<uint16_t>;
}
implementation {
  components DevsSensorC, MainC;
  MainC.SoftwareInit -> DevsSensorC;
  Read = DevsSensorC;
}

