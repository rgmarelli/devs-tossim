/* 
 * devsCPP - a DEVS C++ library
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
#ifndef DEVS_CPP_DEFINES__
#define DEVS_CPP_DEFINES__

#include <limits>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <math.h>

namespace DEVS {

class Time
{
public:
    
    static const unsigned int to_nano_sec = 1000000000;
    static const unsigned int mili_to_nano_sec = 1000000;  

    Time() {
       this->get_current_time();
    }

    Time( unsigned long sec, unsigned long nanosec ) {
        this->time_.tv_nsec = nanosec;
        this->time_.tv_sec = sec;
    }

    Time( unsigned long nanosec ) {
        this->time_.tv_sec = (nanosec / to_nano_sec); 
        this->time_.tv_nsec = (nanosec % to_nano_sec); 
    }

    bool operator > ( const Time& t ) const {
        if( this->sec() > t.sec() ) return true;
        if( (this->sec() == t.sec()) && (this->nsec() > t.nsec() ) ) return true; 
        return false;
    }
    
    bool operator == ( const Time& t ) const {
        return this->sec() == t.sec() &&
               this->nsec() == t.nsec();
    }

    Time operator + ( const Time& t ) const {
        Time r( this->nsec() + t.nsec() );
        r.time_.tv_sec += (this->sec() + t.sec());
        return r;
    }

    Time operator * ( const float x ) const {

        float iptr=0; 
        float d = modff(x, &iptr);

        //Multiply the fractional part of X
        Time r( (d*to_nano_sec)*this->sec() + d*(float)this->nsec() );

        //Multiply the integer part of X
        for( unsigned int i=0; i < iptr; i++ ) {
            r = *this + r;
        }
        return r;
    }

    Time operator - ( const Time& t ) const {
        if( this->nsec() >= t.nsec() ) {
		    Time r( this->nsec() - t.nsec() );
		    r.time_.tv_sec += (this->sec() - t.sec());
            return r;
        
	    }
	    Time r( (this->nsec() - t.nsec()) + to_nano_sec );        
        r.time_.tv_sec += (this->sec() - t.sec() - 1);
        return r;
    }

    Time static infinity() {
        Time t;
        t.time_.tv_sec = std::numeric_limits<long int>::max();
        t.time_.tv_nsec = std::numeric_limits<long int>::max(); 
        return t;
    }

    Time static now() {
        return Time();
    }

    Time static cpu_time() {
        Time t(0, 0);
        t.get_current_time( CLOCK_PROCESS_CPUTIME_ID );
        return t;
    }

    bool static sleep_interval( const Time &interval ) {
      	return nanosleep( &interval.time_, NULL ) == 0;
    }

    time_t sec() const { return this->time_.tv_sec; }
    long nsec() const { return this->time_.tv_nsec; } 

    long msec() const { return this->nsec() / 1000000; }

    std::string to_string() const {
        std::stringstream strtime;
        strtime << this->sec() << "s" << this->nsec() << "nsec";
        return strtime.str();
    }

private:
    void get_current_time(clockid_t clk_id=CLOCK_REALTIME) {
        clock_gettime( clk_id , &this->time_ );
    }

    struct timespec time_;
};

typedef Time TIME;

} // DEVS

#endif

