#include "tcpsocket.hpp"
#include <arpa/inet.h>

class tcpclsocket : public tcpsocket
{
	private:
		
	public:
		int socket_create(int family,int type,int protocol,int port)
		{
			int fd=socket(family,type,protocol);
			if(fd<0)
				er.err_sys("socket error!");
			
			return fd;
		}
		
		int Connect(int fd,struct sockaddr *sa, socklen_t salen)
		{
			if (connect(fd, sa, salen) < 0)
				er.err_sys("connect error");

           return 0;
		}
};
