/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * Este módulo define funciones de soporte que se utilizan para calcular la ecuación de Friis.
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
