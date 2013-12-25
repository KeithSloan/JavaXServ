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
#include "Xwindow.h"
#include <cstring>
extern "C"              // C includes
{
#define NEED_EVENTS
#define NEED_REPLIES
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
}

int windowCount;

// Windows are tracked on the Server and sento the Client
Xwindow::Xwindow(int t, int id, Xwindow *par,int x,int y,int w,int h,int d,int b)
// Xwindow constructor
// t type ( -1 root, 0 window, 1 pixmap)
// par pointer to parent
// x,y offsets
// w,h width, height
// d,b depth border
{
// Is this the root window
windowId  = id;
if ( t == -1 )
   {
   type     = 0;
   previous = parent = NULL;
   last     = this;
   mapState = 2;
   javaId   = 0;
   windowCount = 1;
   colourMap = initialColourMap();
   }
else
   {
   type     = t;
   previous = last;
   parent   = par;
   last -> next = this;
   mapState  = 0;
   javaId    = windowCount;
   colourMap = parent -> getColourMap();
   }
next      = NULL;
last      = this;
eventMask = 0;
xPos      = x;
yPos      = y;
width     = w;
height    = h;
depth     = d;
border    = b;
borderColour = 0;
backGroundColour = 0x00FFFFFF;
firstSub  = lastSub = NULL;
levelNext = levelPrevious = NULL;
numSub = 0;
windowCount++;
std::cerr << "Xwindow constructor : Colour Map : "<< colourMap << std::endl;
}

Xwindow::~Xwindow(void)
{
// Need to update pointers on same level & Globally
}

//void Xwindow::listColourMaps()
//{
//Colormap *ptr;
//int      i,count;

//ptr = XListInstalledColormaps(Xwindow::display,windowId,&count);
//std::cerr << "Installed Colour Maps" << std::endl;
//for ( i = 0; i < count; i++ )
//    {
//    std::cerr << "Colour Map : " << (int) *ptr;
//    ::printf(" 0x%08x\n",(int) *ptr);
//    ptr++;
//    }
//}

int Xwindow::initialColourMap()
{
return(0x20); 
}

// My Ubuntu system only support 24bit colour so we have to do own think for colourmap

//Screen *screen;

//Visual    visual;
//Colormap  cm;

//std::memset(&visual,0,sizeof(visual));
//visual.visualid = 0x21;
//visual.c_class = StaticColor;
//visual.c_class = PseudoColor;
//visual.red_mask = 0xFF0000;
//visual.green_mask = 0xFF00;
//visual.blue_mask = 0xFF;
//visual.bits_per_rgb = 8;
//visual.map_entries = 256;
//std::cerr << "Create Colour Map : Window : "<< windowId << std::endl;
//cm = XCreateColormap(Xwindow::display,windowId,&visual,AllocNone);
//std::cerr << "Colour Map Created: " << cm << " : ";
//::printf("0x%08x\n",(int) cm);
//std::cerr << "Install Colour Map" << std::endl;
//XInstallColormap(Xwindow::display,cm);
//listColourMaps();
//return(cm);
//}
	 
//screen = XDefaultScreenOfDisplay(display);
//std::cerr << "Default Colour Map Depth : " << DefaultDepth(display,0) << std::endl;
//std::cerr << "Default Colour Map : " << XDefaultColormapOfScreen(screen) << std::endl; 
//return(XDefaultColormapOfScreen(screen));
//}


void Xwindow::setEventMask(int ev)
{
eventMask = ev;
if (( eventMask & ExposureMask) == ExposureMask)
   {
   std::cerr << "Expose set" << std::endl;
   }
}


void Xwindow::setColourMap(int cm)
{
colourMap = cm;
}


// Think there maybe an X call to get Colour Maps of Window
// Need to investigate also think more than one colour map per Window
int Xwindow::getColourMap(void)
{
return colourMap;
}

void Xwindow::setBorderColour(int bc)
{
borderColour = bc;
}

void Xwindow::setBackGroundColour(int bc)
{
backGroundColour = bc;
}


void Xwindow::DumpWindow()
{
	std::cerr << "Dump Xwindow " <<
	std::endl;
}
//---------------------------------------------------//
// Search all SubWindows for Wid and return JavaWid  //
//---------------------------------------------------//
int Xwindow::JavaWid(int wid)
{
Xwindow *ptr = next;
int count = 0;
// Is it this window
if ( windowId == wid )
   {
   return(javaId);
   }
else
   {
   // Check remaining windows
   while ( ptr != NULL && ptr -> windowId != wid && count < windowCount )
      {
      ptr = ptr -> next;
      count++;
      }
   if ( ptr -> windowId != wid )
      {
      std::cerr << "**** ERROR **** Window " << wid << " Not found" << std::endl;
      return(-1);
      }
   }
return(ptr -> javaId);
}

//
// Return the X11 Window Id of this Window
//
int Xwindow::X11windowId()
{
	return(windowId);
}
//
// Return Java Windos Id for this window
//
int Xwindow::getJavaWid()
{
	return(javaId);
}


//---------------------------------------------------//
// Search all SubWindows for JavaWid return WindowId //
//---------------------------------------------------//
int Xwindow::X11windowId(int wid)
{
Xwindow *ptr = next;

// Is it this window
if ( javaId == wid )
   {
   return(windowId);
   }
else
   {
   // Check remaining windows
   while ( ptr != NULL && ptr -> javaId != wid )
      {
      ptr = ptr -> next;
      }
   if ( ptr -> javaId != wid )
      {
      std::cerr << "******* ERROR ****** Window " << wid << " Not found" << std::endl;
      return(0);
      }
   }
return(ptr -> windowId);
}

//---------------------------------------------------//
// Find XwindowID and return address of Object       //
//---------------------------------------------------//
Xwindow *Xwindow::AddressWin(int wid)
{
Xwindow *ptr = next;

// Is it this window
if ( windowId == wid )
   {
   return(this);
   }
else
   {
   // Check remaining windows
   while ( ptr != NULL && ptr -> windowId != wid )
      {
      ptr = ptr -> next;
      }
   if ( ptr -> windowId != wid )
      {
      std::cerr << "**** ERROR window " << wid << " Not found" << std::endl;
      return(NULL);
      }
   }
return(ptr);
}

//------------------------------------------------------//
// Find JavewindowID and return address of Object	//
//------------------------------------------------------//
Xwindow *Xwindow::AddressJavaWin(int wid)
{
Xwindow *ptr = next;

// Is it this window
if ( javaId == wid )
   {
   return(this);
   }
else
   {
   // Check remaining windows
   while ( ptr != NULL && ptr -> javaId != wid )
      {
      ptr = ptr -> next;
      }
   if ( ptr -> javaId  != wid )
      {
      std::cerr << "**** ERROR window " << wid << " Not found" << std::endl;
      return(NULL);
      }
   }
return(ptr);
}


//---------------------------------------------------//
// Update Coordinates to relative to root window     //
//---------------------------------------------------//
void Xwindow::rootCoord(int x, int y, short *rx, short *ry)
{
Xwindow *ptr = parent;

while ( ptr != NULL )
  {
  x += ptr -> xPos;
  y += ptr -> yPos;
  ptr = ptr -> parent;
  }
*rx = x + xPos;
*ry = y + yPos;
}

//------------------------------------------------------//
// Create Java Window					                //
//------------------------------------------------------//
int Xwindow::CreateJavaWin()
{
struct
   {
   char  request;
   char  fill1;
   short length;
   int	 parentId;
   int	 javaId;
   short x;
   short y;
   short w;
   short h;
   short b;
   short fill2;
   int   bc;
   int   bgc;
   } javaCreateWin;

std::cerr << " Send Create Java Window for X window: " << windowId 
          << " Parent " << parent -> windowId
          << " Java Id " << nextJavaWin
          << " Parent Java Id : " << parent -> javaId 
          << std::endl;

javaCreateWin.request  = 1;
javaCreateWin.length   = sizeof(javaCreateWin) >> 2;
javaCreateWin.parentId = parent -> javaId;
javaCreateWin.javaId   = javaId = nextJavaWin; 
javaCreateWin.x        = xPos;
javaCreateWin.y        = yPos;
javaCreateWin.w        = width;
javaCreateWin.h        = height;
javaCreateWin.b	       = border;
javaCreateWin.bc       = borderColour;
javaCreateWin.bgc      = backGroundColour;
javasock -> write((char *)&javaCreateWin,sizeof(javaCreateWin));
return(nextJavaWin++);
}

//------------------------------------------------------//
// Create Java PixMap					//
//------------------------------------------------------//
void Xwindow::CreateJavaPixmap()
{
struct
   {
   char  request;
   char  fill1;
   short length;
   int	 javaId;
   short w;
   short h;
   short depth;
   short fill2;
   } createPixmap;
   
std::cerr << " Send Create Java PixMap : " << javaId 
          << " width  " << width
          << " height " << height
          << " depth " << depth
          << std::endl;
createPixmap.request  = 53;
createPixmap.length   = sizeof(createPixmap) >> 2;
createPixmap.javaId   = javaId = nextJavaWin++;
createPixmap.w        = width;
createPixmap.h        = height;
createPixmap.depth    = depth;
javasock -> write((char *) &createPixmap,sizeof(createPixmap));
}


void printHex2(int value)
{
::printf("0x%08x\n",value);
}

//------------------------------------------------------//
// Expose 			        		//
//------------------------------------------------------//
void Xwindow::expose(int num)
{
xEvent event;
int jw;

// Don't think expose depends on mapstate set xclock trace only expose mask
//if ( type == 0 && mapState != 2 )
//   {
   std::cerr << "Expose : Window " << windowId << std::endl;
   printHex2(eventMask);
   jw = JavaWid(windowId);
   // Do we tell Java even if ExposureMask not set ??
   std::cerr << "Send to Java Map window : " << jw << std::endl;
   javasock -> addWindowToExposeMap(jw);
   if (( eventMask & ExposureMask) == ExposureMask)
      {
      mapState = 2;
      // For now expose the whole Window.
      // Coordinates are relative to Window
      std::cerr << "Expose Event:   Width : " << width << "Height : " << std::endl;
      ::memset(&event,0,sizeof(event));
      event.u.u.type = 12;
      event.u.u.sequenceNumber = num;
      event.u.expose.window = windowId;
      event.u.expose.x = 0;
      event.u.expose.y = 0;
      event.u.expose.width = width;
      event.u.expose.height = height;
      X11sock -> write((char *) &event,sizeof(event));
      }
   else
      std::cerr << "Expose Mask not set in EventMask" << std::endl;
//   }
}

//------------------------------------------------------//
// Might not be true after updates for windows mask values 
// This Expose is not dependant on current state	//
// used by ClearArea					//
//------------------------------------------------------//
void Xwindow::exposeMust(int num,int x, int y,int w, int h)
{
xEvent event;

if ( type == 0 )
   {
   // if ( javaId == -1 ) CreateJavaWin();
   mapState = 2;
   if ( w == 0 ) w = width;
   if ( h == 0 ) h = height;
   std::cerr << "Expose : " << x << " : " << y << " : " << w << " : " << h << std::endl;
   std::memset(&event,0,sizeof(event));
   event.u.u.type = 12;
   event.u.u.sequenceNumber = num;
   event.u.expose.window = windowId;
   event.u.expose.x = x;
   event.u.expose.y = y;
   event.u.expose.width = w;
   event.u.expose.height = h;
   X11sock -> write((char *) &event,sizeof(event));
   }
}

Xwindow *Xwindow::getParent(void)
{
return parent;
}

//------------------------------------------------------//
// Return Parent Map state				//
//------------------------------------------------------//
int Xwindow::ParentMapState(void)
{
Xwindow *wrkptr = parent;

std::cerr << "Parent Map State - parent : " << parent << " id " << parent -> windowId << std::endl;
// Old code has bug will loop
// Not sure why need to search back
//while (wrkptr != NULL)
//     {
//     if ( wrkptr -> mapState != 0 )
//        wrkptr = wrkptr -> parent;
//     }
//if ( wrkptr == NULL ) return(2);
//return(1);
std::cerr << "Parent State : " << parent -> mapState << std::endl;
return(parent -> mapState);
}

//------------------------------------------------------//
// Notify Mapped					//
//------------------------------------------------------//
void Xwindow::NotifyMapped(int eid,int num)
{
xEvent event;

std::cerr << "NotifyMapped - event : " << eid << " window : " << windowId << " Sequence Num : " << num << std::endl;
std::memset(&event,0,sizeof(event));
event.u.u.type = 19;
event.u.u.sequenceNumber = num;
event.u.mapNotify.event  = eid;
event.u.mapNotify.window = windowId;
X11sock -> write((char *) &event,sizeof(event));
std::cerr << "Send Map Window Event" << std::endl;
}

//------------------------------------------------------//
// Config Notified					//
//------------------------------------------------------//
void Xwindow::ConfigNotify(int num)
{
xEvent  event;

std::memset(&event,0,sizeof(event));
event.u.u.type = 22;
event.u.u.sequenceNumber = num;
if (( eventMask & StructureNotifyMask ) == StructureNotifyMask )
   event.u.configureNotify.event = parent -> windowId;
else
   event.u.configureNotify.event = windowId;
event.u.configureNotify.window = windowId;
// Where does this come from
event.u.configureNotify.aboveSibling = 0x0180006b;
event.u.configureNotify.x = xPos;
event.u.configureNotify.y = yPos;
event.u.configureNotify.width = width;
event.u.configureNotify.height = height;
X11sock -> write((char *) &event,sizeof(event));
std::cerr << "Send Change Window Notify Event" << std::endl;
}

//------------------------------------------------------//
// Map Window 						//
//------------------------------------------------------//
void Xwindow::MapWindow(int pstate,int num)
{
std::cerr << "Map Window : " << windowId << " mapstate : " << mapState << " Parent State " << pstate << std::endl;
if ( mapState == 2 ) return;  // If already mapped do nothing
if (( eventMask & StructureNotifyMask ) == StructureNotifyMask )
   {
   std::cerr << "Notify Mapped : " << num << std::endl;
   NotifyMapped(windowId,num);
   }
if (( parent -> eventMask & SubstructureNotifyMask ) == SubstructureNotifyMask )
   {
   std::cerr << "Notify Parent : " << parent -> windowId << std::endl;
   NotifyMapped(parent -> windowId,num);
   }
if ( pstate == 2 ) // If parent mapped expose and change state to mapped
   {
   expose(num);
   mapState = 2;
   }
else
   {
   mapState = 1; // Set mapped state to pending
   }
}

//------------------------------------------------------//
// UnMap Window 					//
//------------------------------------------------------//
int Xwindow::UnMapWindow(void)
{
mapState = 0;
return(javaId);
}
 
//------------------------------------------------------//
// Map SubWindows 					//
//------------------------------------------------------//
void Xwindow::MapSubWindows(int num)
{
int count;
Xwindow *ptr;

std::cerr << "Map Sub Windows : " << windowId << std::endl;
count = 0; 
if ( firstSub != NULL )
   {
   ptr = firstSub;
   while ( ptr != NULL )
      {
      ptr -> MapWindow(mapState,num);
      ptr -> MapSubWindows(num);
      ptr = ptr -> levelNext;
      }
   if ( mapState == 2 )
      exposePendingSubWindows(num);
   }
}

//------------------------------------------------------//
// Expose any Pending Sub Windows			//
//------------------------------------------------------//
void Xwindow::exposePendingSubWindows(int num)
{
// Used by mapWindow & MapSubWindows to map subwindows in pending expose state
Xwindow *ptr;

// Does not seem to depend on our Expose Mask
std::cerr << "Expose Pending SubWindows" << std::endl;
if ( firstSub != NULL )
   {
   ptr = firstSub;
   while ( ptr != NULL )
      {
      if ( ptr -> mapState == 1 )
         {
         // add to Send to Java
         ptr -> expose(num);
         }
      ptr -> exposePendingSubWindows(num);
      ptr = ptr -> levelNext;
      }
   }
else
   std::cerr << "No Sub Windows to expose" << std::endl;
}

//------------------------------------------------------//
// Create  SubWindow					//
//------------------------------------------------------//
Xwindow *Xwindow::CreateSubWindow(int id, int x, int y,
                                           int w, int h,int d,int b)
{
Xwindow *newPtr = new Xwindow(0,id,this,x,y,w,h,d,b);

// Is this the first Subwindow
if ( firstSub == NULL )
   {
   firstSub = newPtr;
   }
else
   {
   lastSub -> levelNext = newPtr;
   newPtr  -> levelPrevious = lastSub;
   }
lastSub = newPtr;
numSub++;
return(newPtr);
}

//------------------------------------------------------//
// Create  PixMap					//
//------------------------------------------------------//
Xwindow *Xwindow::CreatePixmap(int id, int w, int h,int d)
{
//Xwindow *newPtr = new Xwindow(1,id,NULL,0,0,0,w,h,d,0);
//std::cerr << "CreatePixmap - id " << id << std::endl;
//return(newPtr);

std::cerr << "CreatePixmap - id " << id << std::endl;
return(new Xwindow(1,id,this,0,0,w,h,d,0));
}

//------------------------------------------------------//
// Delete SubWindows					//
//------------------------------------------------------//
void Xwindow::DeleteSubWindows()
{
Xwindow *ptr = firstSub;

std::cerr << "Delete Sub windows" << std::endl;
while ( ptr != NULL )
   {
   ptr -> DeleteSubWindows();
   ptr = ptr -> next;
   }
delete firstSub;
firstSub = lastSub = NULL;
numSub = 0;
}

//------------------------------------------------------//
// Get Geometry			 			//
//------------------------------------------------------//
void Xwindow::GetGeometry(int root,int num)
{
xGetGeometryReply reply;

std::memset((char *) &reply,0,sizeof(reply));
reply.type   = 1;
reply.depth  = depth;
reply.sequenceNumber = num;
reply.length = 0;
reply.root   = root;
reply.x      = xPos;
reply.y      = yPos;
reply.width  = width;
reply.height = height;
reply.borderWidth = border;
X11sock -> write((char *) &reply,sizeof(reply));
std::cerr << "Send Get Geometry Reply" << std::endl;
}

//------------------------------------------------------//
// Get Window Attributes	 			//
//------------------------------------------------------//
void Xwindow::GetWindowAttributes(int num)
{
xGetWindowAttributesReply reply;

std::memset((char *) &reply,0,sizeof(reply));
reply.type   = 1;
reply.backingStore  = NotUseful;
reply.sequenceNumber = num;
reply.length = 3;
reply.visualID  = javaId;
reply.c_class = 1;	// 1 - InputOuput
reply.bitGravity = ForgetGravity;
reply.winGravity = NorthWestGravity;
reply.backingBitPlanes = 0xFFFFFFFF;
reply.backingPixel = 0;
reply.saveUnder = 0;
reply.mapInstalled = TRUE;
reply.override = 0;  // 0 - unmapped
reply.colormap = 0x21;
reply.allEventMasks = eventMask;
reply.yourEventMask = eventMask;
reply.doNotPropagateMask = 0;
X11sock -> write((char *) &reply,sizeof(reply));
std::cerr << "Send Get Window Attributes Reply" << std::endl;
}

//------------------------------------------------------//
//  QueryTree			 			//
//------------------------------------------------------//
void Xwindow::QueryTree(int root,int num)
{
Xwindow *ptr = next;
xQueryTreeReply reply;
int i = numSub;

std::memset((char *) &reply,0,sizeof(reply));
reply.type   = 1;
reply.sequenceNumber = num;
reply.length    = numSub; 
reply.root      = root;
reply.nChildren = numSub;
X11sock -> write((char *) &reply,sizeof(reply));
while ( i > 0 ) 
   {
   if ( ptr == NULL )
      {
      std::cerr << "Error Number of Sub Windows does not match count" << std::endl;
      }
   if ( ptr -> type == 0 )
      {
      i--;
      X11sock -> write((char *) &ptr -> windowId,4);
      }
   ptr = ptr -> next;
   }
std::cerr << "Send Query Tree Reply" << std::endl;
}

//------------------------------------------------------//
//  ConfigWindow		 			//
//------------------------------------------------------//
void Xwindow::ConfigWindow(int mask,int *ptr)
{
if ((mask & 0x01 ) == 0x01)
   {
   xPos = *(ptr++);
   }
if ((mask & 0x02 ) == 0x02)
   {
   yPos = *(ptr++);
   }
if ((mask & 0x04 ) == 0x04)
   {
   width = *(ptr++);
   }
if ((mask & 0x08 ) == 0x04)
   {
   height = *(ptr++);
   }
}
