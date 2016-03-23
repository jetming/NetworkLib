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

typedef void(*PTRFUNREAD)(const char *data,int fd); //wrt=0:��������Ҫserver��clientд wrt=1��������Ҫserver��clientд
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
			epoll�����ṩ���¼�������
		*/
		//virtual void doSthRead(const char *data,int fd)=0;
		//virtual int doSthWrite(int wfd,const char* data)=0;
		
		void EpollFunc(PTRFUNREAD doSthRead,int listenfd)
		{
			int  maxi,sockfd,epfd,nfds,connfd;
			char line[MAXLINE];
            struct sockaddr_in clientaddr;
			socklen_t clilen=sizeof(struct sockaddr);
			
			//����epoll_event�ṹ��ı���,ev����ע���¼�,�������ڻش�Ҫ������¼�
			struct epoll_event ev,events[20];
			//�������ڴ���accept��epollר�õ��ļ�������
			epfd=epoll_create(256);			

			//������Ҫ������¼���ص��ļ�������
			ev.data.fd=listenfd;
			//����Ҫ������¼�����
			ev.events=EPOLLIN|EPOLLET;
			//ev.events=EPOLLIN;

			//ע��epoll�¼�
			epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

			maxi = 0;

			int bOut = 0;
			
			for ( ; ; )
			{
				if (bOut == 1)
					break;
				//�ȴ�epoll�¼��ķ���

				nfds=epoll_wait(epfd,events,20,-1);
				//�����������������¼�
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
						//�������ڶ��������ļ�������

						setnonblocking(connfd);
						ev.data.fd=connfd;
						//��������ע��Ķ������¼�

						ev.events=EPOLLIN | EPOLLET;
						//ev.events=EPOLLIN;

						//ע��ev
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
							// ȷ��sockfd��nonblocking��
							recvNum = recv(sockfd, data + count, MAXLINE, 0);
							if(recvNum < 0)
							{
								if(errno == EAGAIN)
								{
									// �����Ƿ�������ģʽ,���Ե�errnoΪEAGAINʱ,��ʾ��ǰ�������������ݿɶ�
									// ������͵����Ǹô��¼��Ѵ���.
									bReadOk = true;
									break;
								}
								else if (errno == ECONNRESET)
								{
										// �Է�������RST
										CloseAndDisable(sockfd, events[i]);
										cout << "counterpart send out RST\n";
										break;
								 }
								else if (errno == EINTR)
								{
									// ���ź��ж�
									continue;
								}
								else
								{
									//���������ֲ��Ĵ���
									CloseAndDisable(sockfd, events[i]);
									cout << "unrecovable error\n";
									break;
								}
						   }
						   else if( recvNum == 0)
						   {
								// �����ʾ�Զ˵�socket�������ر�.���͹�FIN�ˡ�
								CloseAndDisable(sockfd, events[i]);
								cout << "counterpart has shut off\n";
								break;
						   }

						   // recvNum > 0
							count += recvNum;
						   if ( recvNum == MAXLINE)
						   {
							   continue;   // ��Ҫ�ٴζ�ȡ
						   }
						   else // 0 < recvNum < MAXLINE
						   {
							   // ��ȫ����
                               line[count]='\0';
							   bReadOk = true;
							   break; // �˳�while(1),��ʾ�Ѿ�ȫ����������
						   }
						   						
						}
						
						if(bReadOk==true)
						{
							doSthRead(line,sockfd);//��read�������ݽ��д���							
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
