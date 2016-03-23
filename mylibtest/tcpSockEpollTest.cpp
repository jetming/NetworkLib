#include "tcpserversocket.hpp"

#define MAXLEN 100
#define PORT 40900

void doSthRead(const char* data,int fd)
{
    printf("%s\n",data);

    write(fd,"i am back!",strlen("i am back!"));
}
int main()
{
    struct sockaddr_in cliaddr;
    char buf[MAXLEN];
    socklen_t clilen;
    struct rio_t* rio;
    Epoll ep;

    tcpserversocket* tss=new tcpserversocket;
    int listenfd=tss->socket_create(AF_INET,SOCK_STREAM,0,40900);

    printf("listenfd:%d\n",listenfd);

    ep.EpollFunc(doSthRead,listenfd);

    return 0;
}
