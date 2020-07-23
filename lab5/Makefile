CC=g++
CFLAGS=-I
CFLAGS+=-Wall
CPPPOSTFIX=.cpp
FILE1=client
FILE2=server

all: client server

client: $(FILE1)$(CPPPOSTFIX)
	$(CC) $(CFLAGS) $(FILE1)$(CPPPOSTFIX) -o $(FILE1)

server: $(FILE2)$(CPPPOSTFIX)
	$(CC) $(CFLAGS) $(FILE2)$(CPPPOSTFIX) -o $(FILE2)

clean:
	rm -f *.o client server /tmp/lab5