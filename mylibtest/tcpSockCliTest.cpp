/*************************************************************************
	> File Name: tcpSockClient.cpp
	> Author: jetming
	> Mail: jetmingx@qq.com
	> Created Time: Sat 23 Apr 2016 06:25:56 PM CST
 ************************************************************************/

#include<iostream>
#include "tcpclisocket.hpp"
#include <stdio.h>

using namespace std;

int main()
{
    tcpclsocket *tcs=new tcpclsocket;
   int clifd= tcs->socket_create(AF_INET,SOCK_STREAM,0,0);
    
    struct sockaddr_in serverAddr, cliaddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(40900);

    struct in_addr* addrptr=(struct in_addr*)malloc(sizeof(struct in_addr*));
    if(inet_aton("127.0.0.1",addrptr))
        serverAddr.sin_addr=*addrptr;
    else{
        printf("inet aton error.\n");
        return -1;
    }

    const u_char *p=(const u_char*)addrptr;
    printf("addr:%d\n",p[0]);
    //connec之前指定本地端口和IP
    /*
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port=htons(60000);
    cliaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bind(clifd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
    */

    tcs->Connect(clifd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    //tcs->Connect(clifd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));  //两次connect，有一次会失败

    if(fork()==0)
    {
        char buffer[1024];
        cin>>buffer;
        tcs->tcpwrite(clifd,buffer,strlen(buffer));
        //close(clifd);//此处使用close关闭时，会导致客户端和服务端都阻塞
        shutdown(clifd,SHUT_WR); //半关闭，告诉服务端：客户端已写完
        //while(1);
        //close(clifd);
        exit(0);
    }else{
        struct rio_t* rio;

        rio=(struct rio_t*)malloc(sizeof(struct rio_t));
        rio->rio_fd=clifd;
        rio->rio_cnt=0;

        int MAXLEN=1024;
        char buf[MAXLEN];
        int n;
        //shutdown(clifd,SHUT_RD);
        while((n=tcs->tcpread(rio,buf,MAXLEN)))
        {
            printf("client read:%d\t%s\n",n,buf);
            //close(clifd);
            //break;
        }
        //while(1);
        exit(0);
    }
    return 0;
}

