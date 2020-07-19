CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES=procMonitor.cpp
FILES+=lab3.cpp

all: procMonitor lab3

procMonitor: procMonitor.cpp
	$(CC) $(CFLAGS) procMonitor.cpp -o procMonitor

lab3: lab3.cpp
	$(CC) $(CFLAGS) lab3.cpp -o lab3

clean:
	rm -f *.o monitor