/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_MOTE_RADIOPARAMS__
#define DEVS_MOTE_RADIOPARAMS__

struct RadioParams
{
    RadioParams(float x0=0, float y0=0, float z0=0) : x(x0), y(y0), z(z0),
                                                      frequency( 2.4 ), // 802.15.4
                                                      gain( 1.9 ), // dBi (Texas Instruments AN040) - max 6 dBi AN2975
                                                      receiver_sensitivity( -85 ), // cc2420
                                                      tx_output_power( -3 )
    {}

    float x; //meters
    float y; //meters
    float z; //meters

    float frequency; //GHz 
    float gain; //dBi

    float receiver_sensitivity; // dBm

    float tx_output_power; // dBm 
};

#endif //DEVS_MOTE_RADIOPARAMS__
