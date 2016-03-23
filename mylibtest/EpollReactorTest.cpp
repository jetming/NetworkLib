#include "Reactor.hpp"
#include "EventHandler.hpp"
#include "Server.hpp"
#include <sys/time.h>
#include <time.h>

void doSthRead(const char* data,int fd)
{
    printf("%s\n",data);
    
    write(fd,"i am back!",strlen("i am back!"));
}

int main()
{
    Eventhandler *evh=new Server;
    Reactor re;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    re.register_handler(evh);
    re.waitEvents(&tv,doSthRead);
}
