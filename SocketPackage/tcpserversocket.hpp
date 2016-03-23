#include "tcpsocket.hpp"
#include <arpa/inet.h>
#include "../Epoll2.hpp"

class tcpserversocket : public tcpsocket
{
	public:
		virtual int socket_create(int family,int type,int protocol,int port)
		{
			int listenfd,connfd;
			struct sockaddr_in cliaddr,seraddr;
			if((listenfd=socket(family,type,protocol))<0)
			{
				er.err_sys("socket error!");
				exit(1);
			}
				
			bzero(&seraddr,sizeof(seraddr));
			seraddr.sin_family=family;
			seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
			seraddr.sin_port=htons(port);
			
			if(bind(listenfd,(struct sockaddr*)&seraddr,sizeof(seraddr))<0)
			{
				er.err_sys("bind error!");
				exit(1);
			}
			
			if(listen(listenfd,LISTENQ)<0)
			{
				er.err_sys("listen error!");
				exit(1);
			}
			
			return listenfd;
		}
		
		int Accept(int fd,struct sockaddr* cliaddr,socklen_t* clilen)
		{
			int		n;

            *clilen=1;
			again:
				if ( (n = accept(fd, cliaddr, clilen)) < 0) {
			#ifdef	EPROTO
					if (errno == EPROTO || errno == ECONNABORTED)
			#else
					if (errno == ECONNABORTED)
			#endif
						goto again;
					else
						er.err_sys("accept error");
				}
				return(n);
		}

};
