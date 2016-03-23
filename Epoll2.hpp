#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

using namespace std;

#ifndef MAXLINE
#define MAXLINE 20
#endif

#define OPEN_MAX 100
#define INFTIM 1000

typedef void(*PTRFUNREAD)(const char *data,int fd); //wrt=0:后续不需要server向client写 wrt=1：后续需要server向client写
//typedef int(*PTRFUNWRITE)(int wfd,const char* data);

class Epoll
{
	public:
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
		
		/*
			epoll对面提供的事件处理函数
		*/
		//virtual void doSthRead(const char *data,int fd)=0;
		//virtual int doSthWrite(int wfd,const char* data)=0;
		
		void EpollFunc(PTRFUNREAD doSthRead,int listenfd)
		{
			int  maxi,sockfd,epfd,nfds,connfd;
			char line[MAXLINE];
            struct sockaddr_in clientaddr;
			socklen_t clilen=sizeof(struct sockaddr);
			
			//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
			struct epoll_event ev,events[20];
			//生成用于处理accept的epoll专用的文件描述符
			epfd=epoll_create(256);			

			//设置与要处理的事件相关的文件描述符
			ev.data.fd=listenfd;
			//设置要处理的事件类型
			ev.events=EPOLLIN|EPOLLET;
			//ev.events=EPOLLIN;

			//注册epoll事件
			epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

			maxi = 0;

			int bOut = 0;
			
			for ( ; ; )
			{
				if (bOut == 1)
					break;
				//等待epoll事件的发生

				nfds=epoll_wait(epfd,events,20,-1);
				//处理所发生的所有事件
				cout << "\nepoll_wait returns\n";
				for(int i=0;i<nfds;i++)
				{
					if(events[i].data.fd==listenfd)
					{
                        bzero((char*)&clientaddr,sizeof(clientaddr));
						connfd=accept(listenfd,(sockaddr*)&clientaddr,&clilen);

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
					
					else if(events[i].events & EPOLLIN)
					{
						cout << "EPOLLIN" << endl;
						if ( (sockfd = events[i].data.fd) < 0)
							continue;
						
						int recvNum = 0;
						int count = 0;
						bool bReadOk = false;
						char *data=line;
						
						while(1)
						{
							// 确保sockfd是nonblocking的
							recvNum = recv(sockfd, data + count, MAXLINE, 0);
							if(recvNum < 0)
							{
								if(errno == EAGAIN)
								{
									// 由于是非阻塞的模式,所以当errno为EAGAIN时,表示当前缓冲区已无数据可读
									// 在这里就当作是该次事件已处理处.
									bReadOk = true;
									break;
								}
								else if (errno == ECONNRESET)
								{
										// 对方发送了RST
										CloseAndDisable(sockfd, events[i]);
										cout << "counterpart send out RST\n";
										break;
								 }
								else if (errno == EINTR)
								{
									// 被信号中断
									continue;
								}
								else
								{
									//其他不可弥补的错误
									CloseAndDisable(sockfd, events[i]);
									cout << "unrecovable error\n";
									break;
								}
						   }
						   else if( recvNum == 0)
						   {
								// 这里表示对端的socket已正常关闭.发送过FIN了。
								CloseAndDisable(sockfd, events[i]);
								cout << "counterpart has shut off\n";
								break;
						   }

						   // recvNum > 0
							count += recvNum;
						   if ( recvNum == MAXLINE)
						   {
							   continue;   // 需要再次读取
						   }
						   else // 0 < recvNum < MAXLINE
						   {
							   // 安全读完
                               line[count]='\0';
							   bReadOk = true;
							   break; // 退出while(1),表示已经全部读完数据
						   }
						   						
						}
						
						if(bReadOk==true)
						{
							doSthRead(line,sockfd);//对read到的数据进行处理							
						}						
					}
										
				}
			}
		}
		
	private:
		void CloseAndDisable(int sockid, epoll_event ee)
		{
			close(sockid);
			ee.data.fd = -1;
		}
};
