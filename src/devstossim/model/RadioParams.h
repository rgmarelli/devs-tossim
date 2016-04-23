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
