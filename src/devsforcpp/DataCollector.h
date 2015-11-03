/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_DATA_COLLECTOR__
#define DEVS_CPP_DATA_COLLECTOR__

#include "Stream.h"
#include "Guard.h"
#include "BasicTCPMessageLayer.h"

namespace DEVS {

class Default_Handshake
{
public:
    bool operator()( Socket *socket ) {
        return true;
    }
};

template <class HANDSHAKE=Default_Handshake>
class DataCollector
{
public:

	DataCollector() : collector_thread_(-1), exit_(false), client_socket_(NULL), server_socket_(NULL)
	{
        pthread_mutexattr_init(&attr_);
        pthread_mutexattr_settype(&attr_, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex_, &attr_);
	}

    ~DataCollector() {
        this->stop();
    }

	bool start( unsigned int port ) {
		if( collector_thread_ != -1 ) return true;
		this->server_socket_ = tcp_.listen( port );
        if( !this->server_socket_ ) {
            Log::write(LOG_ERR,"DEVS","[DataCollector] start error (tcp listen)");
            return NULL;
        }

		return pthread_create(&collector_thread_, NULL, &DataCollector::accept, (void*)this) == 0;
	}

	bool stop() {
		exit_ = true;
        if( this->server_socket_ ) {
            this->tcp_.close( this->server_socket_ );
            if( collector_thread_ != -1 ) {
        		pthread_join( collector_thread_, NULL );
            }
        }
	}

    void disconnect() {
        Guard( this->mutex_ );
        if( this->client_socket_ != NULL ) {
            this->tcp_.disconnect( this->client_socket_ );
        }
    }

	Stream& collected_data() {
		return this->stream_;
	}

	virtual int send(unsigned char* buffer, unsigned int max_size) {
        Guard( this->mutex_ ); // to avoid race conditions when handshaking
        if( this->client_socket_ == NULL ) {
            return -1;
        }
		return this->tcp_.send( this->client_socket_, buffer, max_size );
	}

private:
	static void* accept( void* arg ) {
		DataCollector* collector = (DataCollector*)arg;
        while( !collector->exit_ ) {
            int ready = collector->tcp_.poll( collector->server_socket_ );
            if( ready > 0 ) {
                Socket *socket = collector->tcp_.accept( collector->server_socket_ );
                if( !socket ) {
                    Log::write(LOG_ERR,"DEVS","[DataCollector] accept error (tcp accept)");
                    break;
                }
                if( HANDSHAKE()( socket ) ) {
                    collector->assign_client_socket( socket ); 
                    collector->collect();     
                    collector->assign_client_socket( NULL ); 
                }
                collector->tcp_.disconnect( socket );
            }
            else if( ready == -1 ) {
                Log::write(LOG_ERR,"DEVS","[DataCollector] poll error (tcp accept)");
                break;
            }
        }
        return NULL;
    }

    void assign_client_socket( Socket *socket ) {
        Guard( this->mutex_ );
        this->client_socket_ = socket;
    }    

	void collect() {

		while( !this->exit_ ) {
			unsigned char buffer[512];
			int size = this->tcp_.recv( this->client_socket_, buffer, 512, false );
			if( size > 0 ) {
				//std::cout << "recv: " << size << std::endl;
				this->stream_.write( buffer, size );
			}
			else {
                Log::write(LOG_ERR,"DEVS","[DataCollector] collect error (tcp recv)");
				return;
			} 
		}
	}

private:
	Stream stream_;
	BasicTCPMessageLayer tcp_;
	pthread_t collector_thread_;
	bool exit_;
	Socket *client_socket_;
	Socket *server_socket_;
    pthread_mutex_t mutex_;
    pthread_mutexattr_t attr_;
};

}; //DEVS

#endif //DEVS_CPP_DATA_COLLECTOR__
