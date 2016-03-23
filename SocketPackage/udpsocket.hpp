#include "socket.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>

class udpsocket : public Socket
{
	public:
		int socket_create(int family,int type,int protocol)
		{
			int n;
			if(n=socket(family,type,protocol)<0)
				printf("socket create erro");
			return n;
		}
}