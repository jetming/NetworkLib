#include "tcpserversocket.hpp"

#define MAXLEN 100
#define PORT 40900
int main()
{
    struct sockaddr_in cliaddr;
    char buf[MAXLEN];
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

    int n=tss->tcpread(rio,buf,MAXLEN);
    printf("read_back:%d\t%s\n",n,buf);
    tss->tcpwrite(connfd,buf,n);
}
