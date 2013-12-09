# cat Makefile
CC     = g++
CFLAGS = -c -g -D_BSD
LIBS = -l X11
INCLUDES = -I .
SOURCES = $(wildcard *.C)
OBJECTS = $(SOURCES:.C=.o)

all: JavaXServ

JavaXServ:	$(OBJECTS)
	$(CC) -o JavaXServ $(OBJECTS) $(LIBS)	

%.o : %.C %.h
	$(CC) $(CFLAGS) $<

clean :
	rm -f *.o
	rm -f JavaXServ


