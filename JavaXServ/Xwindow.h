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

#include "tcpSocket.h"

extern "C"              // C includes
{
#include <X11/Xlib.h>
#include <X11/Xregion.h>
#include <errno.h>
}

class Xwindow
   {
   private:
      int     type;			// Type 0 map 1 pixmap
      int     windowId;			// X11 Window ID
      int     javaId;			// Java Graphics Index or -1
      static  Xwindow *last;		// Last Window Pointer
      static  int nextJavaWin;		// Number of Java Windows 
      Xwindow *next;            	// Global Next & Previous
      Xwindow *previous;
      Xwindow *levelNext;		// Next & Previous same level
      Xwindow *levelPrevious;
      Xwindow *firstSub;
      Xwindow *lastSub;
      Xwindow *parent;
      int numSub;			// Number of Real Sub windows
      int xPos,yPos;
      int eventMask;
      int mapState;
      int height,width;
      int depth,border;
      
      void NotifyMapped(int ewid,int num);

   public:
       Xwindow(int t,int id,Xwindow *parent,
                    int em, int x,int y,int w,int h,int depth,int border);
       ~Xwindow();
      static  tcpSocket *javasock;	// java Socket
      static  tcpSocket *X11sock;	// X11 socket
       Xwindow *CreateSubWindow(int id,int em,int x,int y,
                                int w,int h,int depth,int border);
       Xwindow *CreatePixmap(int id,int w,int h,int depth);
       Xwindow *AddressWin(int wid);
       Xwindow *AddressJavaWin(int wid);
       void	   DumpWindow();
       void    DeleteSubWindows();
       int     JavaWid(int wid);
       int	   X11windowId();
       int     X11windowId(int wid);
       int     ParentMapState();
       int     CreateJavaWin();
       void    CreateJavaPixmap();
       void    expose(int num);
       void    exposeMust(int x, int y, int w, int h,int num);
       void    exposePendingSubWindows(int num);
       void    MapWindow(int state,int num);
       void    UnMapWindow(void);
       void    MapSubWindows(int num);
       void    GetGeometry(int root,int num);
       void    QueryTree(int root,int num);
       void    ConfigNotify(int mask,int num);
       void    GetWindowAttributes(int num);
       void    ConfigWindow(int mask, int *ptr);
       void    rootCoord(int x, int y, short *rx, short *ry);
   };
