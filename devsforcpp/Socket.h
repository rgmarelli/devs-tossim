/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_SOCKET__
#define DEVS_CPP_SOCKET__

namespace DEVS {

struct Socket
{
    Socket( int fd ) : sockfd(fd)
    {}

    int sockfd;
};

};

#endif //DEVS_CPP_SOCKET__
