// This file is part of JavaXServ.
// http://SourceForge.Net/p/JavaXServ
// Copyright (C) Keith Sloan 2008-2013 keith@sloan-home.co.uk
//
// JavaXServ is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// JavaXServ is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with JavaXServ.  If not, see <http://www.gnu.org/licenses/>.

extern "C"
{
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
}

#ifndef tcpSocket

class tcpSocket;

class SockSet
   {
   private:
       fd_set    readMask;
       int       maxfd;
       
   public:
		SockSet(void);
        ~SockSet(void);
        void init(void);
        void addReadMask(tcpSocket *);
        int  select(int sec);
        int  isset(tcpSocket *);
   };

class tcpSocket 
   {  
   private:
	long int  sockfd;
	struct    sockaddr_in server; 
	struct    sockaddr_in client; 
	socklen_t clientLen;
	char      Xbuffer[2048];
        int       isrtPoint;
	int       exposeMapCount;
        friend    void SockSet::addReadMask(tcpSocket *);
        friend    int  SockSet::isset(tcpSocket *);

   public:
	tcpSocket(int net,int type);
	tcpSocket(int sock);
	~tcpSocket(void);
	int  bind(int address,int port);
	void setReUse(void);
	void listen(int num);
	int  accept(void);
	int  read(char *buffer,int len);
        int  readBlock(char *buffer,int len);
        void put(char *ptr,int len); 
        void putByte(int b);
        void putInt(int v);
	void assert(void);
        void reset(void);
        void flush(void);
	int  write(char *buffer,int len);
        int  getClientAddr(void);
 	int  getServerAddr(void);
        void error(int err,char *msg);
	int  getError(void);
	void close(void);
        void putExposeMapHeader(void);
        void flushExposeMapHeader(void);
        void addWindowToExposeMap(int w);
   };
#endif
