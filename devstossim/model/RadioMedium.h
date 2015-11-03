/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * El modelo acoplado RadiMedium
 */
#ifndef DEVS_MOTE_RADIOMEDIUM__
#define DEVS_MOTE_RADIOMEDIUM__

#include "RadioParams.h"
#include "Math.h"
#include <CoupledModel.h>

class RadioMedium : public DEVS::CoupledModel
{
public:

    RadioMedium() : CoupledModel("RadioMedium")
    {}

    virtual void register_radio( const std::string &model_name, const RadioParams &params ) {
        this->add( model_name );
        this->model_params_[model_name] = params;
        this->addCouplingForAllModels( "RadioTransmit", DEVS::Port(model_name, "RadioReceive"));
        this->addCouplingForAllModels( DEVS::Port(model_name,"RadioTransmit"), "RadioReceive");
    }

    virtual DEVS::ExternalMessage* translate( DEVS::OutputMessage *message, DEVS::Port &dstPort ) {
        RadioParams srcParam = this->model_params_[message->srcPort().model_name()];
        RadioParams dstParam = this->model_params_[dstPort.model_name()];

        if( radio_model_transmit( srcParam, dstParam ) ) {
            DEVS::ExternalMessage* myMessage = new DEVS::ExternalMessage(dstPort);
            myMessage->putContent(message->content(),message->size());

            std::string hexmsg = myMessage->dump();            
            DEVS::Log::write(LOG_DEBUG,"DEVS", "[%s] On Air (%s -> %s): %s", name().c_str(),
                                               message->srcPort().model_name().c_str(),
                                               myMessage->dstPort().model_name().c_str(),
                                               hexmsg.c_str() );

            return myMessage;
        }
        return NULL;
    }

protected:
    bool radio_model_transmit( const RadioParams &src, const RadioParams &dst ) {
        // Calcular la ecuación de Friis para los dos transceivers.
        float w = Math::wavelength( src.frequency );
        float R = Math::distance( src, dst );
        float Pt = Math::dbm_to_watts( src.tx_output_power );
        float Gt = Math::dbi_to_gain( src.gain );
        float Gr = Math::dbi_to_gain( dst.gain );

        if( R == 0 ) {
            //Sanity check para evitar división por cero.
            return true;
        }

        float Pr = Pt*(Gt*Gr)*pow( w/(4*Math::pi*R), 2 );

        // Comparar el valor calculado de Pr con la potencia de recepción mínima del transceiver
        return ( Pr > Math::dbm_to_watts(dst.receiver_sensitivity) );
    }  

private:
    std::map<std::string, RadioParams> model_params_;

};

#endif
