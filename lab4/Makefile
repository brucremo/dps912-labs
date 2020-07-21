CC=g++
CFLAGS=-I
CFLAGS+=-Wall
CPPPOSTFIX=.cpp
FILE1=intfMonitor
FILE2=sysmonExec

all: monitor sysmonExec

monitor: $(FILE1)$(CPPPOSTFIX)
	$(CC) $(CFLAGS) $(FILE1)$(CPPPOSTFIX) -o $(FILE1)

sysmonExec: $(FILE2)$(CPPPOSTFIX)
	$(CC) $(CFLAGS) $(FILE2)$(CPPPOSTFIX) -o $(FILE2)

clean:
	rm -f *.o monitor