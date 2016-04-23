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
#ifndef DEVS_CPP_GUARD__
#define DEVS_CPP_GUARD__

#include <pthread.h>

namespace DEVS {

class Guard
{
public:
	Guard( pthread_mutex_t mutex ) : mutex_(mutex)
	{
		pthread_mutex_lock( &mutex_ );
	}

	~Guard() {
		pthread_mutex_unlock( &mutex_ );
	}

private:
    pthread_mutex_t& mutex_;
};

}; //DEVS

#endif //GUARD__

