/*************************************************************************
	> File Name: tcpSockClient.cpp
	> Author: jetming
	> Mail: jetmingx@qq.com
	> Created Time: Sat 23 Apr 2016 06:25:56 PM CST
 ************************************************************************/

#include<iostream>
#include "tcpclisocket.hpp"

using namespace std;

int main()
{
    tcpclsocket *tcs=new tcpclsocket;
   int clifd= tcs->socket_create(AF_INET,SOCK_STREAM,0,0);
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(40900);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    tcs->Connect(clifd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

    if(fork()==0)
    {
        char buffer[1024];
        cin>>buffer;
        tcs->tcpwrite(clifd,buffer,strlen(buffer));
        //close(clifd);//此处使用close关闭时，会导致客户端和服务端都阻塞
        shutdown(clifd,SHUT_WR); //半关闭，告诉服务端：客户端已写完
        exit(0);
    }else{
        struct rio_t* rio;

        rio=(struct rio_t*)malloc(sizeof(struct rio_t));
        rio->rio_fd=clifd;
        rio->rio_cnt=0;

        int MAXLEN=1024;
        char buf[MAXLEN];
        int n;
        while((n=tcs->tcpread(rio,buf,MAXLEN)))
            printf("client read:%d\t%s\n",n,buf);

        exit(0);
    }
    return 0;
}

