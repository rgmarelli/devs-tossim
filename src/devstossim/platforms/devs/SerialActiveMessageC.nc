// $Id: ActiveMessageC.nc,v 1.6 2008/04/24 06:52:12 klueska Exp $
/*
 * Copyright (c) 2005-2006 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/**
 * Dummy implementation to support the null platform.
 */

configuration SerialActiveMessageC {
  provides {
    interface SplitControl;

    interface AMSend[uint8_t id];
    interface Receive[uint8_t id];
    interface Receive as Snoop[uint8_t id];

    interface Packet;
    interface AMPacket;
    interface PacketAcknowledgements as Acks;

  }
}
implementation {
  components DevsSerialActiveMessageC as AM;
  components MainC;

  MainC.SoftwareInit -> AM;

  SplitControl = AM.SplitControl;
  AMSend = AM;
  Receive = AM.Receive;
  Snoop = AM.Snoop;
  Packet = AM;
  AMPacket = AM;
  Acks = AM.Acks;
}

