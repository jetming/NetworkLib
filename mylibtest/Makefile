CC=g++
CFLAG=-g -Wno-deprecated #屏蔽警告信息
PROGARMNAME=mylibtest
LIB=/root/mylib/SocketPackage
LIB2=/root/mylib/EpollReactor

all:tcpSockTest tcpSockEpollTest

tcpSockTest:tcpSockTest.cpp
	$(CC) $(CFLAG) tcpSockTest.cpp -I $(LIB)
	mv ./a.out ./tcpSockTest
	mv ./tcpSockTest ./build

tcpSockEpollTest:tcpSockEpollTest.cpp
	$(CC) $(CFLAG) tcpSockEpollTest.cpp -I $(LIB)
	mv ./a.out ./tcpSockEpollTest
	mv ./tcpSockEpollTest ./build
EpollReactorTest:EpollReactorTest.cpp
	$(CC) $(CFLAG) EpollReactorTest.cpp -I $(LIB2)
	mv ./a.out ./EpollReactorTest
	mv ./EpollReactorTest ./build

clean:
	rm -f *.o
