/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef __DEVS_MOTE_TRANSCEIVER__
#define __DEVS_MOTE_TRANSCEIVER__

/* 
Informacion del CC2420 (oficial): http://focus.ti.com/docs/prod/folders/print/cc2420.html
La referencia a utilizar es el data-sheet.
*/
#include <AtomicModel.h>
#include <string>
#include <stdint.h>

const std::string TRANSCEIVER_RECEIVING = "receiving";
const std::string TRANSCEIVER_TRANSMIT = "transmit";
const std::string TRANSCEIVER_TRANSMITTING = "transmitting";
const std::string TRANSCEIVER_IDLE = "idle";
const std::string TRANSCEIVER_SLEEP = "sleep";
const std::string TRANSCEIVER_OFF = "off";

const std::string TRANSCEIVER_TURNING_OFF = "turning_off";
const std::string TRANSCEIVER_TURNING_ON = "turning_on";

class Transceiver : public DEVS::AtomicModel
{
public:
    unsigned int sendcount;

    Transceiver(const std::string name, float duty_cycle ) : AtomicModel(name), message_in_(NULL), message_out_(NULL), 
                                                             wakeup_interval_( DEVS::Time::infinity() ), 
                                                             start_stop_interval_( 1.3*DEVS::Time::mili_to_nano_sec ), /* 
                                                                                       CC2420 spec: Before using the radio in either RX or TX mode, 
                                                                                       the voltage regulator and crystal oscillator must be turned on 
                                                                                       and become stable (see state diagram in the spec) */
                                                             duty_cycle_( duty_cycle ),
                                                             data_rate_( 250 /* Kbit/s - maximum data rate from 802.15.4 */ )
    {
        /* Intercambio de paquetes hacia el medio */
        outputPorts().add( DEVS::Port(name,"RadioTransmit",1) );
        inputPorts().add( DEVS::Port(name,"RadioReceive",1) );
        sendcount=0;
        /* Intercambio de paquetes con TinyOS. O mejor dicho: con la capa MAC */
        inputPorts().add( DEVS::Port(name,"AMSend",1) );
        outputPorts().add( DEVS::Port(name,"AMSendDone",1) );
        outputPorts().add( DEVS::Port(name,"AMReceive",1) );

        /* Para poder apagarlo y prenderlo con TinyOS */
        inputPorts().add( DEVS::Port(name,"TurnOff",1) );
        inputPorts().add( DEVS::Port(name,"TurnOn",1) );

        outputPorts().add( DEVS::Port(name,"TurnOffDone",1) );
        outputPorts().add( DEVS::Port(name,"TurnOnDone",1) );

        registerPhase(TRANSCEIVER_RECEIVING);
        registerPhase(TRANSCEIVER_TRANSMIT);
        registerPhase(TRANSCEIVER_TRANSMITTING);
        registerPhase(TRANSCEIVER_IDLE);
        registerPhase(TRANSCEIVER_SLEEP);
        registerPhase(TRANSCEIVER_OFF);

        registerPhase(TRANSCEIVER_TURNING_ON);
        registerPhase(TRANSCEIVER_TURNING_OFF);

        setPhase(TRANSCEIVER_OFF);
    }

    virtual void internalTransition() {
        
        // Transmit
        if( phaseIs(TRANSCEIVER_TRANSMIT) ) {
            setSigma( this->transfer_interval( message_out_->size() ) );
            setPhase(TRANSCEIVER_TRANSMITTING);
            return;
        }

        if( phaseIs(TRANSCEIVER_TRANSMITTING) ) {
            setSigma( this->radio_active_interval() );
            setPhase(TRANSCEIVER_IDLE);
            return;
        }

        // Receive
        if( phaseIs(TRANSCEIVER_RECEIVING) ) {
            setSigma( this->radio_active_interval() );
            setPhase(TRANSCEIVER_IDLE);
            return;
        }

        // Transceiver turning ON/OFF
        if( phaseIs(TRANSCEIVER_TURNING_ON) ) {
            setSigma( this->radio_active_interval() );
            setPhase(TRANSCEIVER_IDLE);
            return;
        }

        if( phaseIs(TRANSCEIVER_TURNING_OFF) ) {
            setSigma(DEVS::Time::infinity());
            setPhase(TRANSCEIVER_OFF);
            return;
        }

        //Duty-cycle
        if( phaseIs(TRANSCEIVER_SLEEP) ) {
            setSigma( this->radio_active_interval() );
            setPhase(TRANSCEIVER_IDLE);
            return;
        }

        if( phaseIs(TRANSCEIVER_IDLE) ) {
            setSigma(this->wakeup_interval_);
            setPhase(TRANSCEIVER_SLEEP);
            return;
        }
    }

    // Funcion de transicion externa (delta Ext)
    virtual void externalTransition(DEVS::ExternalMessage* message) {

        DEVS::Log::write(LOG_DEBUG,"DEVS", "[%s] externalTransition phase=%s, dstPort=%s",
                         name().c_str(),phase().c_str(),message->dstPort().name().c_str());

        if(  message->dstPort().nameIs("AMSend") ) {
            sendcount++;
        }

        // Transceiver turn ON/OFF
        if( phaseIs(TRANSCEIVER_OFF) && message->dstPort().nameIs("TurnOn") ) {
            /* TinyOS ordena start mediante split control y debe monitorear una interrupcion
               para generar el evento de startDone */
            setSigma(this->start_stop_interval_);
            unsigned long wakeup_interval = message->content<uint16_t>()*DEVS::Time::mili_to_nano_sec;
            if( wakeup_interval > 0 ) {
                this->wakeup_interval_ = wakeup_interval;
            }
            else {
                this->wakeup_interval_ = DEVS::Time::infinity();
            }
            setPhase(TRANSCEIVER_TURNING_ON);
            return;
        }

        if( ( phaseIs(TRANSCEIVER_IDLE) || phaseIs(TRANSCEIVER_SLEEP) )  && message->dstPort().nameIs("TurnOff") ) {
            setSigma(this->start_stop_interval_);
            setPhase(TRANSCEIVER_TURNING_OFF);
            return;
        }

        // Transceiver transmit
        if( ( phaseIs(TRANSCEIVER_IDLE) || phaseIs(TRANSCEIVER_SLEEP) || phaseIs(TRANSCEIVER_RECEIVING) ) && message->dstPort().nameIs("AMSend") ) {
            if( phaseIs(TRANSCEIVER_RECEIVING) ) {
                DEVS::Log::write(LOG_DEBUG,"DEVS", "[%s] Trying to send but receiving, will stop receiving and start transmission.",name().c_str());
                delete(this->message_in_);
            }

            setPhase(TRANSCEIVER_TRANSMIT);
            setSigma(0);
            this->message_out_ = new DEVS::OutputMessage(outputPorts().getPortByName("RadioTransmit"));
            this->message_out_->putContent( message->content(), message->size() );
            DEVS::Log::write(LOG_INFO,"DEVS", "[%s] Transmmiting message...",name().c_str());
            return;
        }

        // Transceiver transmit interference
        if( phaseIs(TRANSCEIVER_TRANSMITTING) && message->dstPort().nameIs("RadioReceive") ) {
            DEVS::Log::write(LOG_DEBUG,"DEVS", "[%s] Transmmit interference detected...",name().c_str());
            return;
        }

        // Transceiver receive
        if( phaseIs(TRANSCEIVER_IDLE) && message->dstPort().nameIs("RadioReceive") ) {
            //printf(" message.content_size() = %d\n", message->size() );
            this->message_in_ = new DEVS::OutputMessage(outputPorts().getPortByName("AMReceive"));
            this->message_in_->putContent( message->content(), message->size() );

            setSigma(this->transfer_interval( message_in_->size() ));
            setPhase(TRANSCEIVER_RECEIVING);
            DEVS::Log::write(LOG_INFO,"DEVS", "[%s] Receiving message...",name().c_str());
            return;
        }

        // Transceiver receive interference
        if( phaseIs(TRANSCEIVER_RECEIVING) && message->dstPort().nameIs("RadioReceive") ) {
            setSigma( this->radio_active_interval() );
            setPhase(TRANSCEIVER_IDLE);
            delete( this->message_in_ );
            DEVS::Log::write(LOG_INFO,"DEVS", "[%s] Message lost: Interference detected.",name().c_str());
            return;
        }

        // Transceiver receive interference
        if( phaseIs(TRANSCEIVER_SLEEP) && message->dstPort().nameIs("RadioReceive") ) {
            DEVS::Log::write(LOG_INFO,"DEVS", "[%s] Message lost: Transceiver is sleeping.",name().c_str());
            return;
        }
    }

    // Funcion de salida (Y)
    virtual DEVS::OutputMessage* outputFunction() {
        if( phaseIs(TRANSCEIVER_TRANSMIT) ) {
            //printf(" message_out_.content_size() = %d\n", message_out_->size() );
            return this->message_out_;
        }

        if( phaseIs(TRANSCEIVER_TRANSMITTING) ) {
            sendcount--;
            return new DEVS::OutputMessage(outputPorts().getPortByName("AMSendDone"));
        }

        if( phaseIs(TRANSCEIVER_RECEIVING) ) {
            //printf(" message_in_.content_size() = %d\n", message_in_->size() );
            return this->message_in_;
        }

        // Signal Turn ON/OFF to TinyOS
        if( phaseIs(TRANSCEIVER_TURNING_ON) ) {
            DEVS::Log::write(LOG_INFO,"DEVS", "[%s] TurnOnDone. LocalWakeupInterval=%s,DutyCycle=%f",name().c_str(),this->wakeup_interval_.to_string().c_str(),this->duty_cycle_);
            return new DEVS::OutputMessage(outputPorts().getPortByName("TurnOnDone"));
        }
        if( phaseIs(TRANSCEIVER_TURNING_OFF) ) {
            return new DEVS::OutputMessage(outputPorts().getPortByName("TurnOffDone"));
        }

        return NULL;
    }

private:
    DEVS::Time transfer_interval( unsigned long data_size ) {
        double msec = (data_size*8)/(this->data_rate_);
        return DEVS::Time( msec*DEVS::Time::mili_to_nano_sec);
    }

    DEVS::Time radio_active_interval() {
        if( this->duty_cycle_ == 1 || this->wakeup_interval_ == DEVS::Time::infinity() ) {
            // Duty-cycling OFF
            return DEVS::Time::infinity();
        }
        // Radio wakeup interval: after each interval, the node will wakeup and check for radio activity
        return this->wakeup_interval_*( this->duty_cycle_/(1 - this->duty_cycle_) );
    }

    DEVS::OutputMessage *message_in_;
    DEVS::OutputMessage *message_out_;
    DEVS::Time wakeup_interval_;
    float duty_cycle_;
    DEVS::Time start_stop_interval_;
    unsigned long data_rate_; // kbit/s
};

#endif
