CC=g++
CFLAG=-g -Wno-deprecated #屏蔽警告信息
PROGARMNAME=mylibtest
LIB=/home/root/mylib/SocketPackage
LIB2=/home/root/mylib/EpollReactor

all:tcpSockServerTest tcpSockCliTest tcpSockEpollTest

tcpSockServerTest:tcpSockServerTest.cpp
	$(CC) $(CFLAG) tcpSockServerTest.cpp -I $(LIB)
	mv ./a.out ./tcpSockServerTest
	mv ./tcpSockServerTest ./build

tcpSockCliTest:tcpSockCliTest.cpp
	$(CC) $(CFLAG) tcpSockCliTest.cpp -I $(LIB)
	mv ./a.out ./tcpSockClient
	mv ./tcpSockClient ./build
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
