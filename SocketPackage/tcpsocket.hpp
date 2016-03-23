#include "socket.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include "../error.hpp"

#define RIO_BUFFSIZE 8192
#define LISTENQ 1024

struct rio_t
{
	int rio_fd; //file description
	int rio_cnt;//unread bytes
	char *buffptr;//next unread bytes
	char rio_buff[RIO_BUFFSIZE];//buffer
};
		
class tcpsocket : public Socket
{
	public:		
		error er;
    public:
	/*
        int socket_create(int family,int type,int protocol)
        {
            int n;
            if ((n=socket(family, type, protocol)) < 0)
                er.err_sys("socket error");			
            return(n);
        }
	*/
		
		virtual int socket_create(int family,int type,int protocol,int port)=0;
		
		int tcpread(struct rio_t* rio,char *userbuf,size_t n)
		{
			int cnt;
            bzero(rio->rio_buff,sizeof(rio->rio_buff));
			while(rio->rio_cnt<=0) //<=0 代表读失败，继续进行读
			{
				rio->rio_cnt=read(rio->rio_fd,rio->rio_buff,RIO_BUFFSIZE);
				if(rio->rio_cnt<0)
                {
					if(errno!=EINTR)
						return -1;
                }
				else if(rio->rio_cnt==0) //读完
                    return 0;
				else
                {
                    rio->buffptr=rio->rio_buff;
                }
			}
			cnt=n;
			
			if(rio->rio_cnt<n)
				cnt=rio->rio_cnt;
			if(cnt<0)
				er.err_sys("read erro!");
			memcpy(userbuf, rio->buffptr, cnt);
			rio->buffptr += cnt;
			rio->rio_cnt -= cnt;
			
			return cnt;
		}
		
		int tcpwrite(int fd,void* buffer,int n)
		{
            int m;
			if((m=write(fd,buffer,n))<n)
			{
				er.err_sys("write erro!");
			}

            return m;
		}
		
		int tcpclose(int fd)
		{
			if(close(fd)==-1)
				er.err_sys("close error!");

            return 0;
		}
};
