#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <unistd.h>
#include <stdlib.h>

class Socket
{
    private:
        int socket_;
    public:
        virtual int socket_create(int family,int type,int protocol,int port)=0;
        //virtual int read(int fd,void *buff,size_t nbytes,int flags)=0;
};

#endif
