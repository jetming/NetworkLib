#include "socket.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>

class tcpsocket : public Socket
{
    public:
        int socket_creat(int family,int type,int protocol)
        {
            int n;
            if ((n=socket(family, type, protocol)) < 0)
                printf("socket error");
            return(n);
        }
};
