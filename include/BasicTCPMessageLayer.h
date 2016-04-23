/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_BASICTCPMESSAGELAYER__
#define DEVS_CPP_BASICTCPMESSAGELAYER__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include "Socket.h"
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <poll.h>

#include "Log.h"

namespace DEVS {

class BasicTCPMessageLayer 
{
public:

    BasicTCPMessageLayer() {
        signal(SIGPIPE, SIG_IGN);
    }

    virtual Socket* listen( unsigned int port, unsigned int backlog=128 ) {

        struct sockaddr_in serv_addr;
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons((u_short)port);
        
        int sockfd = -1;
        if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            return NULL;
        }

        int tr=1;
        setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int));

        if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
            Log::write(LOG_ERR,"DEVS","[TCP] ERR BIND: %s",strerror(errno));
            return NULL;
        }

        if(::listen(sockfd, backlog) == -1) {
            Log::write(LOG_ERR,"DEVS","[TCP] ERR LISTEN");
            return NULL;
        }

        return new Socket(sockfd);
    }

    virtual Socket* accept(Socket* acceptor) {
        struct sockaddr_in cli_addr;
        unsigned clilen = sizeof(cli_addr);
        int clientsock = ::accept(acceptor->sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if(clientsock < 0) {
            return NULL;
        }

        return new Socket(clientsock);
    }

    virtual void close(Socket* acceptor) {
        ::close( acceptor->sockfd );
    }

    virtual Socket* connect( const char* addr, unsigned int port ) {

        struct sockaddr_in serv_addr;
        struct hostent *ptr_server; 

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons((u_short)port);
        
        ptr_server = gethostbyname(addr);
        if ( ((char *)ptr_server) == NULL ) {   
    	    return NULL;       
        }
        
        int sockfd = -1;
        memcpy(&serv_addr.sin_addr, ptr_server->h_addr, ptr_server->h_length);
        if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            return NULL;
        }

        if (::connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
            ::close(sockfd);
            return NULL;
        }
        return new Socket(sockfd);
    }

    virtual int send(Socket* peer, unsigned char* buffer, unsigned int max_size) {
        if(peer->sockfd == -1) return -1;

        int total=0;
        while(total < max_size) { 
            int escritos = ::write(peer->sockfd, &buffer[total], max_size - total);
            if(escritos == -1) {
                return -1;
            }
            total+=escritos;
        }
        return 0;
    }

    virtual int recv(Socket* peer, unsigned char* buffer, unsigned int max_size, bool recv_max=true ) {
        // recv_max: si es true, recibir max_size bytes.

        if(peer->sockfd == -1) return -1;

        int total=0;
        while( total==0 || (recv_max && total < max_size) ) { 
           int leidos = ::read(peer->sockfd, &buffer[total], max_size - total);
           if(leidos <= 0) {
               if( leidos == -1 ) {
                   Log::write(LOG_ERR,"DEVS","[TCP] ERR RECV  %s (%d, %d)",strerror(errno), errno, total);
		           return -1;
               }
               else {
                   Log::write(LOG_ERR,"DEVS","[TCP] CONNECTION CLOSED %s (%d, %d)",strerror(errno), errno, total);
                   return -1;
               }
           }
           total+=leidos;
        }
        return total;
    }

    virtual void disconnect(Socket* peer) {
        if ( peer->sockfd != -1 ) {
            ::close( peer->sockfd );
            peer->sockfd = -1;
        }
    }

    int poll( Socket* peer, int timeout=500 ) {
        struct pollfd fds[1];
        fds[0].fd = peer->sockfd;
        fds[0].events = POLLIN;
        int timeout_msecs = timeout;
        return ::poll(fds, 1, timeout_msecs);
    }

}; 

}; //DEVS

#endif //DEVS_CPP_BASICTCPMESSAGELAYER__
