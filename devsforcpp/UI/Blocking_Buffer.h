/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_UI_BLOCKING_BUFFER__
#define DEVS_CPP_UI_BLOCKING_BUFFER__

#include "Optional.h"
#include <pthread.h>

namespace DEVS {
namespace UI {

template <class E>
class Blocking_Buffer
{
public:
    Blocking_Buffer() : empty_(true) {
        pthread_mutex_init(&mutex_, NULL); 
        pthread_cond_init(&cond_, NULL); 
    }

    Optional<E> get( bool block ) {
        pthread_mutex_lock( &mutex_ );
        while( empty_ && block ) {
            pthread_cond_wait( &cond_, &mutex_ );
        }
        if( !empty_ ) {
            Optional<E> e(e_);
            empty_ = true;
            pthread_mutex_unlock( &mutex_ ); 
            return e;
        }
        pthread_mutex_unlock( &mutex_ );
        return Optional<E>();
    }

    void put( const E& e ) {
        pthread_mutex_lock( &mutex_ );
        e_ = e;
        empty_ = false;

        pthread_mutex_unlock( &mutex_ );
        pthread_cond_signal( &cond_ );
    }

private:
    bool empty_;
    E e_;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_;  
};

} //namespace UI
} //namespace DEVS

#endif //DEVS_CPP_UI_BLOCKING_BUFFER__
