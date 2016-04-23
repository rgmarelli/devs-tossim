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

