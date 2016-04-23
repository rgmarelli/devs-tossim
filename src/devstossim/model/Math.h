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
 * This module defines some support functions used to calculate the Friis equation
 */

#ifndef DEVS_MOTE_MATH__
#define DEVS_MOTE_MATH__

#include <math.h>

namespace Math {
    template<class T> float distance( const T& r1, const T& r2 ) {
        float x = pow( r1.x - r2.x, 2 );
        float y = pow( r1.y - r2.y, 2 );
        float z = pow( r1.z - r2.z, 2 );
        return sqrt( x + y + z );
    }

    static const double pi = 3.1415926;

    float wavelength( float frequency /*GHz*/ ) {
        // wavelength in meters
        double c = 2.99792458;

        /*
            c*10^8 = speed of ligth
            f*10^9 = freq in Hz

                         (c*10^8)     c       c     1
            wavelength = -------- = ------ = --- * ---
                         (f*10^9)   (f*10)    f     10
        */
        return (c/frequency)/10;
    }

    float dbm_to_watts( float dbm ) 
    {
        return pow( 10, dbm/10 ) / 1000;
    }

    float dbi_to_gain( float dbi )
    {
        return pow( 10, dbi/10 );
    }

}

#endif //DEVS_MOTE_MATH__
