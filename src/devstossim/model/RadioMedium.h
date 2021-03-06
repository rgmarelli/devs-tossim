/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * El modelo acoplado RadiMedium
 */
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
        // Calculate the Friis equation for the two transceivers.
        float w = Math::wavelength( src.frequency );
        float R = Math::distance( src, dst );
        float Pt = Math::dbm_to_watts( src.tx_output_power );
        float Gt = Math::dbi_to_gain( src.gain );
        float Gr = Math::dbi_to_gain( dst.gain );

        if( R == 0 ) {
            //Sanity check to avoid division by zero
            return true;
        }

        float Pr = Pt*(Gt*Gr)*pow( w/(4*Math::pi*R), 2 );

        // Compare the calculated reception power "Pr" with the minimun required power of the transceiver
        return ( Pr > Math::dbm_to_watts(dst.receiver_sensitivity) );
    }  

private:
    std::map<std::string, RadioParams> model_params_;

};

#endif
