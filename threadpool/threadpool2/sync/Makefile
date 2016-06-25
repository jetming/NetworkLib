CC=g++
CFLAG = -o

all :  sync

sync : sync.cpp MutexTest.cpp
	$(CC) $(CFLAG) MutexTest sync.cpp MutexTest.cpp -lpthread

clean :
	@rm -f *.o





