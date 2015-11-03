/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * This source code file has use TossimActiveMessageC.nc by Philip Levis 
 * as a reference implementation.
 *
 *
 * DEVS-TOSSIM ActiveMessageC implementation.
 *
 * PacketAcknowledgements are not implemented. 
 */

module DevsActiveMessageC {
    provides {
        interface SplitControl;

        interface AMSend[uint8_t id];
        interface Receive[uint8_t id];
        interface Receive as Snoop[uint8_t id];

        interface Packet;
        interface AMPacket;
        interface PacketAcknowledgements as Acks;

        interface LowPowerListening as lpl;

        interface Init;
    }
}
implementation {


    /*  
    typedef nx_struct message_t {
      nx_uint8_t header[sizeof(message_header_t)];
      nx_uint8_t data[TOSH_DATA_LENGTH];
      nx_uint8_t footer[sizeof(message_footer_t)];
      nx_uint8_t metadata[sizeof(message_metadata_t)];
    } message_t;
    */

    message_t buffer;
    message_t* bufferPtr = &buffer;
    message_t* lastmsg = NULL;
    uint16_t localWakeupInterval = 0;
    uint16_t remoteWakeupInterval = 0;

    void startDone(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {
        signal SplitControl.startDone(SUCCESS);
    }

    void amSendDone(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {
        signal AMSend.sendDone[call AMPacket.type(lastmsg)](lastmsg, SUCCESS);
    }

    void amReceive(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {

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
        registerExternalEvent("AMTurnOnDone",&startDone);  
        registerExternalEvent("AMSendDone",&amSendDone);
        registerExternalEvent("AMReceive",&amReceive);
        return SUCCESS;
    }

    command error_t SplitControl.start() {
        putOutputMessage("AMTurnOn","AMTurnOn",&localWakeupInterval,sizeof(uint16_t));
        return SUCCESS;
    }

    command error_t SplitControl.stop() {
        return SUCCESS;
    }

    command error_t AMSend.send[uint8_t id](am_addr_t addr, message_t* msg, uint8_t len) {
        devs_am_header_t* header = (devs_am_header_t*)&(msg->header);
        header->type = id;
        header->dest = addr;
        header->src = call AMPacket.address();
        header->length = len;

        lastmsg = msg;
        putOutputMessage("AMSend","AMSend",msg,sizeof(message_t));
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
        devs_am_header_t* header = (devs_am_header_t*)&(msg->header);
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
        devs_am_header_t* header = (devs_am_header_t*)&(msg->header);
        header->length = len;
    }

    command am_addr_t AMPacket.address() {
        return TOS_NODE_ID;
    }

    command am_addr_t AMPacket.destination(message_t* amsg) {
        devs_am_header_t* header = (devs_am_header_t*)&(amsg->header);
        return header->dest;
    }

    command bool AMPacket.isForMe(message_t* amsg) {
        return (call AMPacket.destination(amsg) == call AMPacket.address() ||
	            call AMPacket.destination(amsg) == AM_BROADCAST_ADDR);
    }

    command am_id_t AMPacket.type(message_t* amsg) {
        devs_am_header_t* header = (devs_am_header_t*)&(amsg->header);
        return header->type;
    }

    command void AMPacket.setDestination(message_t* amsg, am_addr_t addr) {

        devs_am_header_t* header = (devs_am_header_t*)&(amsg->header);
        header->dest = addr;

    }

    command void AMPacket.setType(message_t* amsg, am_id_t t) {
        devs_am_header_t* header = (devs_am_header_t*)&(amsg->header);
        header->type = t;
    }

    command am_addr_t AMPacket.source(message_t* amsg) {
        devs_am_header_t* header = (devs_am_header_t*)&(amsg->header);
        return header->src;
    }

    command void AMPacket.setSource(message_t* amsg, am_addr_t addr) {
        devs_am_header_t* header = (devs_am_header_t*)&(amsg->header);
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

    /**
    * Tell a protocol that when it sends this packet, it should use synchronous
    * acknowledgments.
    * The acknowledgment is synchronous as the caller can check whether the
    * ack was received through the wasAcked() command as soon as a send operation
    * completes.
    *
    * @param 'message_t* ONE msg' - A message which should be acknowledged when transmitted.
    * @return SUCCESS if acknowledgements are enabled, EBUSY
    * if the communication layer cannot enable them at this time, FAIL
    * if it does not support them.
    */
    async command error_t Acks.requestAck( message_t* msg ) {
        return FAIL;
    }

    /**
    * Tell a protocol that when it sends this packet, it should not use
    * synchronous acknowledgments.
    *
    * @param 'message_t* ONE msg' - A message which should not be acknowledged when transmitted.
    * @return SUCCESS if acknowledgements are disabled, EBUSY
    * if the communication layer cannot disable them at this time, FAIL
    * if it cannot support unacknowledged communication.
    */
    async command error_t Acks.noAck( message_t* msg ) {
        return SUCCESS;
    }

    /**
    * Tell a caller whether or not a transmitted packet was acknowledged.
    * If acknowledgments on the packet had been disabled through noAck(),
    * then the return value is undefined. If a packet
    * layer does not support acknowledgements, this command must return always
    * return FALSE.
    *
    * @param 'message_t* ONE msg' - A transmitted message.
    * @return Whether the packet was acknowledged.
    *
    */
    async command bool Acks.wasAcked(message_t* msg) {
        return FALSE;
    }

    /**
    * Set this this node's radio wakeup interval, in milliseconds. After
    * each interval, the node will wakeup and check for radio activity.
    *
    * Note: The wakeup interval can be set to 0 to indicate that the radio
    * should stay on all the time but in order to get a startDone this
    * should only be done when the duty-cycling is off (after a stopDone).
    *
    * @param intervalMs the length of this node's Rx check interval, in [ms]
    */
    command void lpl.setLocalWakeupInterval(uint16_t intervalMs) {
        localWakeupInterval = intervalMs;
    }

    /**
    * @return the local node's wakeup interval, in [ms]
    */
    command uint16_t lpl.getLocalWakeupInterval() {
        return localWakeupInterval;
    }

    /**
    * Configure this outgoing message so it can be transmitted to a neighbor mote
    * with the specified wakeup interval.
    * @param 'message_t* ONE msg' Pointer to the message that will be sent
    * @param intervalMs The receiving node's wakeup interval, in [ms]
    */
    command void lpl.setRemoteWakeupInterval(message_t *msg, uint16_t intervalMs) {
        //printf("lpl.setRemoteWakeupInterval\n");
        remoteWakeupInterval = intervalMs;
    }

    /**
    * @param 'message_t* ONE msg'
    * @return the destination node's wakeup interval configured in this message
    */
    command uint16_t lpl.getRemoteWakeupInterval(message_t *msg) {
        //printf("lpl.getRemoteWakeupInterval\n");
        return remoteWakeupInterval;
    }
}
