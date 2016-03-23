#ifndef EVENTHANDLER
#define EVENTHANDLER
#include <sys/epoll.h>

typedef void(*PTRFUNREAD)(const char *data,int fd);

class Eventhandler
{
	public:
		virtual int handle_event(struct epoll_event event,PTRFUNREAD dosth)=0;
		virtual int get_handler()=0;
};

#endif
