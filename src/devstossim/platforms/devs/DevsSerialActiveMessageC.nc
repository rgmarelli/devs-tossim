/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 *
 * DEVS-TOSSIM SerialActiveMessage implementation.
 */

module DevsSerialActiveMessageC {
    provides {
        interface SplitControl;

        interface AMSend[uint8_t id];
        interface Receive[uint8_t id];
        interface Receive as Snoop[uint8_t id];

        interface Packet;
        interface AMPacket;
        interface PacketAcknowledgements as Acks;

        interface Init;
    }
}
implementation {

    message_t buffer;
    message_t* bufferPtr = &buffer;
    message_t* lastmsg = NULL;

    void serialStartDone(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {
        signal SplitControl.startDone(SUCCESS);
    }

    void serialAmSendDone(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {
        signal AMSend.sendDone[call AMPacket.type(lastmsg)](lastmsg, SUCCESS);
    }

    void serialAmReceive(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {

        uint8_t len;
        void* payload;

        memcpy(bufferPtr, content, sizeof(message_t));
        len = call Packet.payloadLength(bufferPtr);
        payload = call Packet.getPayload(bufferPtr, call Packet.maxPayloadLength());

        if (call AMPacket.isForMe(bufferPtr)) {
            bufferPtr = signal Receive.receive[call AMPacket.type(bufferPtr)](bufferPtr, payload, len);
        }
        else {
            bufferPtr = signal Snoop.receive[call AMPacket.type(bufferPtr)](bufferPtr, payload, len);
        }
    }

    default event message_t* Receive.receive[am_id_t id](message_t* msg, void* payload, uint8_t len) {
        return msg;
    }

    default event message_t* Snoop.receive[am_id_t id](message_t* msg, void* payload, uint8_t len) {
        return msg;
    }

    command error_t Init.init() {
        registerExternalEvent("SerialAMTurnOnDone",&serialStartDone);  
        registerExternalEvent("SerialAMSendDone",&serialAmSendDone);
        registerExternalEvent("SerialAMReceive",&serialAmReceive);
        return SUCCESS;
    }

    command error_t SplitControl.start() {
        putOutputMessage("SerialAMTurnOn","SerialAMTurnOn",NULL,0);
        return SUCCESS;
    }

    command error_t SplitControl.stop() {
        return SUCCESS;
    }

    command error_t AMSend.send[uint8_t id](am_addr_t addr, message_t* msg, uint8_t len) {

        serial_header_t* header = (serial_header_t*)&(msg->header);
        header->type = id;
        header->dest = addr;
        header->src = TOS_NODE_ID;
        header->length = len;

        lastmsg = msg;
        putOutputMessage("SerialAMSend","SerialAMSend",msg,sizeof(message_t));
        return SUCCESS;
    }

    command error_t AMSend.cancel[uint8_t id](message_t* msg) {
        return FAIL;
    }

    command uint8_t AMSend.maxPayloadLength[uint8_t id]() {
        return call Packet.maxPayloadLength();
    }

    command void* AMSend.getPayload[uint8_t id](message_t* msg, uint8_t len) {
        return call Packet.getPayload(msg, len);
    }

    command void Packet.clear(message_t* msg) {
    }

    command uint8_t Packet.payloadLength(message_t* msg) {
        serial_header_t* header = (serial_header_t*)&(msg->header);
        return header->length;
    }

    command uint8_t Packet.maxPayloadLength() {
        return TOSH_DATA_LENGTH;
    }

    command void* Packet.getPayload(message_t* msg, uint8_t len) {
        if (len <= TOSH_DATA_LENGTH) {
            return msg->data;
        }
        else {
            return NULL;
        }
    }

    command void Packet.setPayloadLength(message_t* msg, uint8_t len) {
        serial_header_t* header = (serial_header_t*)&(msg->header);
        header->length = len;
    }

    command am_addr_t AMPacket.address() {
        return TOS_NODE_ID;
    }

    command am_addr_t AMPacket.destination(message_t* amsg) {
        serial_header_t* header = (serial_header_t*)&(amsg->header);
        return header->dest;
    }

    command bool AMPacket.isForMe(message_t* amsg) {
        return (call AMPacket.destination(amsg) == call AMPacket.address() ||
	            call AMPacket.destination(amsg) == AM_BROADCAST_ADDR);
    }

    command am_id_t AMPacket.type(message_t* amsg) {
        serial_header_t* header = (serial_header_t*)&(amsg->header);
        return header->type;
    }

    command void AMPacket.setDestination(message_t* amsg, am_addr_t addr) {
        serial_header_t* header = (serial_header_t*)&(amsg->header);
        header->dest = addr;

    }

    command void AMPacket.setType(message_t* amsg, am_id_t t) {
        serial_header_t* header = (serial_header_t*)&(amsg->header);
        header->type = t;
    }

    command am_addr_t AMPacket.source(message_t* amsg) {
        serial_header_t* header = (serial_header_t*)&(amsg->header);
        return header->src;
    }

    command void AMPacket.setSource(message_t* amsg, am_addr_t addr) { 
        serial_header_t* header = (serial_header_t*)&(amsg->header);
        header->src = addr;
    }

    command am_group_t AMPacket.group(message_t* amsg) {
        return 0;
    }

    command void AMPacket.setGroup(message_t* amsg, am_group_t grp) { 
    }

    command am_group_t AMPacket.localGroup() {
        return 0;
    }

    async command error_t Acks.requestAck( message_t* msg ) {
        return SUCCESS;
    }

    async command error_t Acks.noAck( message_t* msg ) {
        return SUCCESS;
    }

    async command bool Acks.wasAcked(message_t* msg) {
        return FALSE;
    }
}
