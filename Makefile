.PHONY:all


.PHONY:clean

.PHONY:exception

exception: DException.h  DException.cpp
	g++ DException.cpp -o DException.o

all:

clean:
	rm *.o
