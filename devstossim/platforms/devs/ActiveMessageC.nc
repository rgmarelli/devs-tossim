/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 *
 * ActiveMessageC support for the devs platform (DEVS-TOSSIM).
 */

configuration ActiveMessageC {
  provides {
    interface SplitControl;

    interface AMSend[uint8_t id];
    interface Receive[uint8_t id];
    interface Receive as Snoop[uint8_t id];

    interface Packet;
    interface AMPacket;
    interface PacketAcknowledgements as Acks;

    interface LowPowerListening;

  }
}
implementation {
  components DevsActiveMessageC as AM;
  components MainC;

  MainC.SoftwareInit -> AM;

  SplitControl = AM.SplitControl;
  AMSend = AM;
  Receive = AM.Receive;
  Snoop = AM.Snoop;
  Packet = AM;
  AMPacket = AM;
  Acks = AM.Acks;
  LowPowerListening = AM.lpl;
}

