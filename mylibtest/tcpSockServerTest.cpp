#include "tcpserversocket.hpp"

#define MAXLEN 100
#define PORT 40900
int main()
{
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    struct rio_t* rio;

    tcpserversocket* tss=new tcpserversocket;
    int listenfd=tss->socket_create(AF_INET,SOCK_STREAM,0,40900);

    printf("listenfd:%d\n",listenfd);
    int connfd=tss->Accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);

    printf("connfd:%d\n",connfd);

    rio=(struct rio_t*)malloc(sizeof(struct rio_t));
    rio->rio_fd=connfd;
    rio->rio_cnt=0;

    while(1)
    {
        char buf[MAXLEN];
        int n=tss->tcpread(rio,buf,MAXLEN);
        if(n!=0)
        {   printf("server read:%d\t%s\n",n,buf);
            tss->tcpwrite(connfd,buf,n);
        }else
            break;
    }
}
