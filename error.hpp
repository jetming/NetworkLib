#include <iostream>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

#define MAXLINE     4096

class error
{

    private:
        void err_doit(int errnoflag, int level, const char *fmt, va_list ap)
        {
		    int		errno_save, n;
		    char	buf[MAXLINE + 1];

	        errno_save = errno;		/* value caller might want printed */
	        vsnprintf(buf, MAXLINE, fmt, ap);	/* safe */
	        n = strlen(buf);
	        if (errnoflag)
		        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
	        strcat(buf, "\n");

            daemon_proc=0;
	        if (daemon_proc) {
                //const char* cbuf=buf;
	        	//syslog(level,cbuf);
	        } else {
		        fflush(stdout);		/* in case stdout and stderr are the same */
	        	fputs(buf, stderr);
		        fflush(stderr);
	        }
	        return;
         }
    public:
        void err_sys(const char *fmt, ...)
        {
            va_list ap;

            va_start(ap,fmt);
            err_doit(1,LOG_ERR,fmt,ap);
            va_end(ap);
            exit(1);
        }

        void err_quit(const char *fmt, ...)
        {
            va_list     ap;

            va_start(ap, fmt);
            err_doit(0, LOG_ERR, fmt, ap);
            va_end(ap);
            exit(1);
        }

    private:
        int daemon_proc;
};
