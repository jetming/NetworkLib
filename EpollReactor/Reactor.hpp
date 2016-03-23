#include "EventHandler.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

using namespace std;

class Reactor
{
	public:
		int register_handler(Eventhandler *handler)
		{
            handler_=handler;
			struct epoll_event ev;			
			
			epfd=epoll_create(256);
			epoll_fd_=handler_->get_handler();
			
			ev.data.fd=epoll_fd_;
			ev.events=EPOLLIN|EPOLLET;
			
			epoll_ctl(epfd,EPOLL_CTL_ADD,epoll_fd_,&ev);
			
			return 0;
		}
		
		void remove(Eventhandler *handler)
		{
            handler_=handler;
			//delete handler_;
		}
		
		void waitEvents(timeval *time,PTRFUNREAD doSth)
		{
			int ndfs,connfd;
			struct epoll_event ev,events[20];
			struct sockaddr_in clientaddr;
			socklen_t clilen=sizeof(struct sockaddr);
			for(;;)
			{
				ndfs=epoll_wait(epfd,events,20,-1); //set timeout according to time args
				
				for(int i=0;i<ndfs;i++)
				{
					if(events[i].data.fd==epoll_fd_)
					{
						bzero((char*)&clientaddr,sizeof(clientaddr));
						connfd=accept(epoll_fd_,(struct sockaddr*)&clientaddr,&clilen);

						if(connfd<0){
							perror("connfd<0");
							return;
						}
							

						char *str = inet_ntoa(clientaddr.sin_addr);
						cout << "accapt a connection from " << str << endl;
						//设置用于读操作的文件描述符

						setnonblocking(connfd);
						ev.data.fd=connfd;
						//设置用于注测的读操作事件

						ev.events=EPOLLIN | EPOLLET;
						//ev.events=EPOLLIN;

						//注册ev
						epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);	
					}
					
					if(events[i].events & EPOLLIN)
					{
					handler_->handle_event(events[i],doSth);
					}
				}
			}
		}
		 
	private:
		void setnonblocking(int sock)
		{
			int opts;
			opts=fcntl(sock,F_GETFL);
			if(opts<0)
			{
				perror("fcntl(sock,GETFL)");
				return;
			}
			opts = opts|O_NONBLOCK;
			if(fcntl(sock,F_SETFL,opts)<0)
			{
				perror("fcntl(sock,SETFL,opts)");
				return;
			}
		}
		
		void CloseAndDisable(int sockid, epoll_event ee)
		{
			close(sockid);
			ee.data.fd = -1;
		}
		
	private:
		int epoll_fd_;
		int epfd;
		Eventhandler *handler_;
};
