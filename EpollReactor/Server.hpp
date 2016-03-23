#include "EventHandler.hpp"
#include "../SocketPackage/tcpserversocket.hpp"

#define PORT 40900

class Server : public Eventhandler
{
    public:
		int handle_event(struct epoll_event event,PTRFUNREAD doSthRead)
		{
            int sockfd;
            char line[MAXLINE];

			if(event.events & EPOLLIN)
			{
				cout << "EPOLLIN" << endl;
				if ( (sockfd = event.data.fd) < 0)
					return -1;
							
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
							CloseAndDisable(sockfd, event);
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
							CloseAndDisable(sockfd, event);
							cout << "unrecovable error\n";
							break;
						}
					}
					else if( recvNum == 0)
					{
						// 这里表示对端的socket已正常关闭.发送过FIN了。
						CloseAndDisable(sockfd, event);
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
            return 0;
		}
		
		int get_handler()
		{
			tcpserversocket* tss=new tcpserversocket;
			int listenfd=tss->socket_create(AF_INET,SOCK_STREAM,0,PORT);

			printf("listenfd:%d\n",listenfd);
			//int connfd=tss->Accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);

			//printf("connfd:%d\n",connfd);
			
			return listenfd;
		}

    private:
        void CloseAndDisable(int sockid, epoll_event ee)
        {
            close(sockid);
            ee.data.fd = -1;
        }
};
