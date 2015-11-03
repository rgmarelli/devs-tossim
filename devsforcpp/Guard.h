/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
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

