# cat Makefile
CC     = g++
CFLAGS = -g -D_BSD
LIBS = -l X11
INCLUDES = -I .
COBJS	= Xwindow tcpSocket
EXECS   = JavaXServ 

all:     Xwindow.o tcpSocket.o JavaXServ

Xwindow.o:	Xwindow.C Xwindow.h
	$(CC) -c $(CFLAGS) $(INCLUDES) Xwindow.C

tcpSocket.o:	tcpSocket.C tcpSocket.h
	$(CC) -c $(CFLAGS) $(INCLUDES) tcpSocket.C

JavaXServ.o:	JavaXServ.C
	$(CC) -c $(CFLAGS) $(INCLUDES) JavaXServ.C

JavaXServ:	JavaXServ.o tcpSocket.o Xwindow.o
	$(CC) -o JavaXServ  JavaXServ.o tcpSocket.o Xwindow.o $(LIBS)
	
clean:	
	rm *.o
	rm JavaXServ

tar:
	tar -cvf servIV.tar *
	compress servIV.tar

demo:
	cp javaServ ../demo
	strip ../demo/javaServ
	cp *.class ../demo
	cp SubClass/*.class ../demo/SubClass
	cp test.html ../demo

dos:    tar
	doswrite servIV.tar.Z servIV.trz

src:	
	doswrite -a javaServ.C javaServ.C
	doswrite -a server.java server.jva
	doswrite -a test.html test.html
	doswrite -a tcpSocket.h tcpSocket.h
	doswrite -a tcpSocket.C tcpSocket.C
	doswrite -a Xwindow.h Xwindow.h
	doswrite -a Xwindow.C Xwindow.C
	doswrite -a SubClass/Xrequest.java Xrequest.jva
	doswrite -a SubClass/Xwindow.java Xwindow.jva
	doswrite -a SubClass/MapLayout.java MapLayout.jva
	doswrite -a SubClass/Xsocket.java Xsocket.jva
	doswrite -a SubClass/Trace.java Trace.jva
	doswrite -a Makefile Makefile
	doswrite -a keymap.C keyMap.C 

