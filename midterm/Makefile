CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES=child.cpp
FILES+=parent.cpp

all: child parent

child: child.cpp
	$(CC) $(CFLAGS) child.cpp -o child

parent: parent.cpp
	$(CC) $(CFLAGS) parent.cpp -o parent

clean:
	rm -f *.o child parent