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
#ifndef DEVS_CPP_STREAM__
#define DEVS_CPP_STREAM__

#include "Guard.h"
#include <list>
#include <pthread.h>
#include <string.h>

namespace DEVS {

class Stream
{
public:
	static const unsigned int CHUNKSIZE = 64;
	typedef std::list< unsigned char* > type_list;

	Stream() : size_(0)
	{
	    pthread_mutexattr_init(&attr_);
   	    pthread_mutexattr_settype(&attr_, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex_, &attr_);
		
		this->v_ = new type_list;
	}

	unsigned int read( unsigned char* buffer, unsigned int max_size ) {
		Guard( this->mutex_ );

		if( this->size_ == 0 || max_size == 0 || buffer == NULL ) return 0;

		//unsigned char* first = v_->front();
		unsigned int size_to_copy = (this->size_ > max_size) ? max_size : this->size_; 

		unsigned int chunks_to_copy = size_to_copy / CHUNKSIZE;
		for( unsigned int i=0; i < chunks_to_copy; i++ ) {
			unsigned char* first = v_->front();

			memcpy( &buffer[CHUNKSIZE*i], first, CHUNKSIZE );

			this->remove_front();	
		}

		unsigned int remaining_size = size_to_copy - CHUNKSIZE*chunks_to_copy;
		if( remaining_size > 0 ) {
			unsigned char* first = v_->front();
			memcpy(&buffer[CHUNKSIZE*chunks_to_copy], first, remaining_size);
			this->compact_chunks( remaining_size );
		}

		return size_to_copy;
	}
	
	void write( unsigned char* buffer, unsigned int size ) {		
		Guard( this->mutex_ );

		unsigned int used = tail();
		unsigned int remaining_size = CHUNKSIZE - used;

		if ( this->size_ == 0 ) {
			v_->push_back( new unsigned char[CHUNKSIZE] );
		}

		unsigned char* last = this->v_->back();

		if( size > remaining_size ) {
			if( remaining_size > 0 ) {
				memcpy( &last[used], buffer, remaining_size );
				this->size_ += remaining_size;
			}
			v_->push_back( new unsigned char[CHUNKSIZE] );
			return this->write( &buffer[remaining_size], size - remaining_size );
		}
		else {
			memcpy( &last[used], buffer, size );
			this->size_ += size;
		}
	}

	unsigned int size() {
		Guard( this->mutex_ );
		return size_;
	}

	unsigned char operator [](unsigned int index) {
		Guard( this->mutex_ );

		if( index >= this->size_ ) return 0;
		unsigned int chunk = index / CHUNKSIZE;		

		type_list::iterator it = v_->begin();
		if( chunk > 0 ) {
			std::advance(it, chunk);
		}

		return (*it)[index-chunk*CHUNKSIZE];
	}

private:
	void remove_front() {
		delete( this->v_->front() );
		this->v_->pop_front();
		if( this->size_ > CHUNKSIZE ) {
			this->size_ -= CHUNKSIZE;
		}
		else {
			this->size_ = 0;
		}
	}

	void compact_chunks( unsigned int size ) {

		if( this->size_ <= size || size == CHUNKSIZE ) {
			this->remove_front();
			return;
		}

		if( this->size_ <= CHUNKSIZE ) {
			unsigned int remaining = this->size_ - size;
			unsigned char* first = v_->front();
			memmove( first, &first[size], remaining );
			this->size_ = remaining;
			return;
		}

		type_list* old_v = this->v_;
		unsigned int old_tail = this->tail();

		this->size_ = 0;
		this->v_ = new type_list;

		unsigned char* first = old_v->front();
		this->write( &first[size], CHUNKSIZE - size );
		delete( old_v->front() );
		old_v->pop_front();

		unsigned int total = old_v->size();
		for( unsigned int i = 0; i < total-1; i++ ) {
			unsigned char* first = old_v->front();
			this->write( first, CHUNKSIZE );
			delete( old_v->front() );
			old_v->pop_front();
		}
		first = old_v->front();
		this->write( first, old_tail );
		delete( old_v->front() );
		old_v->pop_front();
	}

	unsigned int tail() {
		/* Used space of the last buffer */
		if( size_ == 0 ) return 0;
		return (size_ - (v_->size()-1)*CHUNKSIZE);
	}

private:
	unsigned int size_;
	type_list* v_;
    pthread_mutex_t mutex_;
	pthread_mutexattr_t attr_;
};

};

#endif //DEVS_CPP_STREAM__

