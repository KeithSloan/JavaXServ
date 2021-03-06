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

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcpSocket.h"

extern "C"		// C includes
{
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
}

extern int errno;

SockSet::SockSet(void)
{
maxfd = 0;
memset((char *) &readMask,0,sizeof(readMask));
}

void SockSet::init(void)
{
std::cerr << "SockSet init : " << std::endl;
maxfd = 0;
memset((char *) &readMask,0,sizeof(readMask));
}

int SockSet::select(int secs)
{
struct timeval delay;

std::cerr << "select maxfd : " << maxfd << std::endl;
delay.tv_sec  = secs;
delay.tv_usec = 0;
return(::select(maxfd,&readMask,(fd_set *) 0, (fd_set *) 0,&delay));
}

int SockSet::wait(void)
{

std::cerr << "wait maxfd : " << maxfd << std::endl;
return(::select(maxfd,&readMask,(fd_set *) 0, (fd_set *) 0,NULL));
}


void SockSet::addReadMask(tcpSocket *s)
{
int i = s -> sockfd + 1;

std::cerr << "Add Read Mask : " << s -> sockfd << std::endl;
maxfd = (maxfd > i) ? maxfd : i;
FD_SET(s -> sockfd,&readMask);
}

void SockSet::removeReadMask(tcpSocket *s)
{
std::cerr << "Remove Read Mask : " << s -> sockfd << std::endl;
FD_CLR(s -> sockfd,&readMask);
}

int SockSet::isset(tcpSocket *s)
{
return(FD_ISSET(s -> sockfd,&readMask));
}

tcpSocket::tcpSocket(int net,int type)
{
setbuf(stdout,NULL);

std::cerr << "Opening Socket" << std::endl;
if ((sockfd = socket(net,type,0)) < 0 )
   {
   error(errno,"Cannot open socket Stream");
   }
isrtPoint = 0;
}

tcpSocket::tcpSocket(int sock)
{
sockfd = sock;
isrtPoint = 0;
}

void tcpSocket::setSocket(int sock)
{
sockfd = sock;
isrtPoint = 0;
}

tcpSocket::~tcpSocket(void)
{
std::cerr << "Closing Socket : "<< sockfd << std::endl;
if ( sockfd != NULL ) ::close(sockfd);
}

void tcpSocket::setReUse(void)
{
int conn = 1;
std::cerr << "Set Kernal ReUse Socket : " << sockfd << std::endl;
::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &conn, sizeof(conn));
}

void tcpSocket::setBroadcastEnable(void)
{
int broadcastEnable = 1;
std::cerr << "Set Broadcast : " << sockfd << std::endl;
::setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
}


int tcpSocket::setAddress(int address,int port)
{
memset((char *) &server,0,sizeof(server));
server.sin_family      = AF_INET;
server.sin_addr.s_addr = htonl(address);
server.sin_port        = htons(port);
std::cerr << "Set Address to : " << inet_ntoa(server.sin_addr) << " port " << port << std::endl;
}

int tcpSocket::getClientAddress(void)
{
std::cerr << "Address is : " << inet_ntoa(client.sin_addr) << std::endl;
return(client.sin_addr.s_addr);
}

void tcpSocket::getHostName(char *name,int len)
{
::gethostname(name,len);
std::cerr << "Host Name  is : " << name << std::endl;
}

int tcpSocket::getHostIP(void)
{
char hostname[120];

struct hostent *he;
struct in_addr **addr_list;
char *ip;
int i;
    
getHostName(hostname,120);         
if ( (he = ::gethostbyname( hostname ) ) == NULL) 
   {
   // get the host info
   std::cerr << "Error in gethostname" << std::endl;
   return -1;
   }
 
addr_list = (struct in_addr **) he -> h_addr_list;
     
//for(i = 0; addr_list[i] != NULL; i++) 
//   {
   //Return the first one;
//   strcpy(ip , inet_ntoa(*addr_list[i]) );
//   return 0;
//
//    }
for(i = 0; addr_list[i] != NULL; i++) 
   {
   //Return the first one;
   ip = inet_ntoa(*addr_list[i]);
   std::cerr << "Host IP " << ip << std::endl;
   return ((int) addr_list[i] -> s_addr);
   }
}


int tcpSocket::bind(int address,int port)
{
// Chnage to use Set Address
std::cerr << "Binding to : " << address << " port " << port << std::endl;

//memset((char *) &server,0,sizeof(server));
//server.sin_family      = AF_INET;
//server.sin_addr.s_addr = htonl(address);
//server.sin_port        = htons(port);
std::cerr << "Binding to :" << address << " port " << port << std::endl;
setAddress(address,port);
return(::bind(sockfd,(struct sockaddr*) &server,sizeof(server)));
}

void tcpSocket::listen(int num)
{
std::cerr << "Listening" << std::endl;
if (::listen(sockfd,num) < 0)
   {
   error(errno,"Server failed to listen");
   }
}

int tcpSocket::accept(void)
{
int newfd;
std::cerr << "accepting" << std::endl;
clientLen = sizeof(client);
errno = 0;
if ((newfd = ::accept(sockfd,(struct sockaddr *) &client,&clientLen)) < 0 )
   {
   error(errno,"accept error"); 
   }
return(newfd);
}

int tcpSocket::read(char *buffer,int len)
{
int r;
//std::cerr << "Reading Length : " << len << std::endl;
r = ::read(sockfd,buffer,len);
// std::cerr << "Bytes Read : " << r << std::endl;
return(r);
}

int tcpSocket::readBlock(char *buffer,int len)
{
int i,j;
// std::cerr << "Read Block length : " << len << std::endl;

i = 0;
j = 1;
while ( i < len)
   {
   j = read(buffer,len - i);
   std::cerr << "Bytes read : " << j << std::endl;
   if ( j <= 0 )
      {
      //close();
      return(j);
      }
   buffer += j;
   i += j;
   }
return(i);
}

void tcpSocket::put(char *ptr,int len)
{
int i,l;

while ( isrtPoint + len > 2048 )
   {
   l = 2048 - isrtPoint;
   ::memcpy(&Xbuffer[isrtPoint],ptr,l);
   write(Xbuffer,2048); 
   ptr += l;
   len -= l;
   isrtPoint = 0;
   }
if ( len > 0 )
   {
   ::memcpy(&Xbuffer[isrtPoint],ptr,len);
   isrtPoint += len;
   }
}

void tcpSocket::putInt(int v)
{
::memcpy(&Xbuffer[isrtPoint],&v,4);
isrtPoint += 4;
if ( isrtPoint == 2048 )
   {
   write(Xbuffer,2048);
   isrtPoint = 0;
   }
}

void tcpSocket::putByte(int b)
{
Xbuffer[isrtPoint++] = b;
if ( isrtPoint == 2048 )
   {
   write(Xbuffer,2048);
   isrtPoint = 0;
   }
}

void tcpSocket::flush(void)
{
if ( isrtPoint > 0 ) write(Xbuffer,isrtPoint);
isrtPoint = 0;
}

void tcpSocket::assert(void)
{
if ( isrtPoint != 0 )
   {
   std::cerr << "Buffer pointer not zero - missing flush ?" << isrtPoint << std::endl;
   exit(2);
   }
}

void tcpSocket::reset(void)
{
isrtPoint = 0;
}

int tcpSocket::sendTo(char *buffer,int len)
{
std::cerr << "sendTo" << std::endl;
if(::sendto(sockfd,buffer,len,0,(struct sockaddr *) &server,sizeof(server)) < 0)
   error(errno,"sendToerror"); 

}

int tcpSocket::receiveFrom(char *buffer,int len)
{
std::cerr << "receiveFrom" << std::endl;
return(::recvfrom(sockfd,buffer,len,0,NULL,NULL));
}


int tcpSocket::write(char *buffer,int len)
{
int ret;
std::cerr << "Writing : " << len << " Bytes" << std::endl;
if ( sockfd != 0 )
   {
   ret = ::write(sockfd,buffer,len);
   }
else
   {
   ret = ::write(1,buffer,len);
   }
if (ret != len)
   {
   std::cerr << "Error Writing to Socket "<< std::endl;
   exit(2);
   }
return(ret);
}

void tcpSocket::close(void)
{
::close(sockfd);
sockfd = NULL;
}

int tcpSocket::getClientAddr(void)
{
return(client.sin_addr.s_addr);
}

int tcpSocket::getServerAddr(void)
{
return(server.sin_addr.s_addr);
}

int tcpSocket::getError(void)
{
return(errno);
}

void tcpSocket::error(int err,char *msg)
{
std::cerr << msg << " : " << err << std::endl;
exit(3);
}

void tcpSocket::putExposeMapHeader()
{
exposeMapCount = 0;
assert();
putByte(8);		// opcode X_MapWindow = 8
putByte(0);		// parm
putByte(0);		// Dummy length
putByte(0);
}

void tcpSocket::flushExposeMapHeader()
{
if ( exposeMapCount > 0 )
   {
   addWindowToExposeMap(0);        // End of Windows marker
   flush();
   }
else
   reset();
}


void tcpSocket::addWindowToExposeMap(int w)
{
std::cerr << "Window Added to Send : " << w << std::endl;
putInt(w);
exposeMapCount++;
}
