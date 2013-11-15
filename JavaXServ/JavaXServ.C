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
#include <unistd.h>
// #include "tcpSocket.h"
#include "Xwindow.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

extern "C"              // C includes
{
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#define NEED_REPLIES
#define NEED_EVENTS
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <errno.h>
}

#define PORT        6789
#define MAXFONTS      64
#define MAXFONTNAME  256
#define STACKSIZE   1024
#define FIRSTKEY       9
#define TIMEOUT	      600

#define DEBUG

#include "keymap.C"

//--------------------------------------//
// Externals
//--------------------------------------//
extern int errno;
extern char **environ;
//--------------------------------------//
// Globals
//--------------------------------------//
char      byteOrder;
int       setupPid;
tcpSocket *X11Serv;
tcpSocket *javaServ;
int       sequenceNum = 0;
Xwindow   *rootWin;
//--------------------------------------//
// Declare static variables             //
//--------------------------------------//
Xwindow*   Xwindow::last = rootWin;
int        Xwindow::nextJavaWin = 1;
tcpSocket* Xwindow::javasock = NULL;	// Socket to listen for Jave Clients
tcpSocket* Xwindow::X11sock  = NULL;	// Socket to Listen for X Clients
//--------------------------------------//
// Globals for X calls & related
//--------------------------------------//
Display *display;
char    *displayVariable;
struct
   {
   int   fid;
   Font  font;
   int   pointSize;
   } fontTable[MAXFONTS];
int fontEntries;

XFontStruct *fontFix;
//--------------------------------------//

void dumpHex(char *ptr,int len)
{
	int i;
	
	for(i=0; i < len; i++)
	   {
	   ::printf(" %2x",*ptr++);
	   }
	::printf("\n");
}

void sendReplyHdr(int words )
{
struct
  {
  char type;
  char data;
  CARD16 sequenceNumber;
  CARD32 length;
  } reply;

reply.type = 1;
reply.data = 0;
reply.sequenceNumber = sequenceNum;
reply.length = words - 8;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
}

int getEventMask(int mask,int *ptr)
{
int i;

if (( mask & 0x800) == 0x800)
   {
   for ( i = 0 ; i < 11 ; i++)
       {
       if ((mask & 1) == 1)
          ptr++;
       mask = mask >> 1;
       }
   return(*ptr);
   }
return(0);
}

int initialColourMap()
{
	// For now just use the default colourMap
	// When working and able to test change to create initial Colormap
	// So that support concurrent requests
	
	// display is a global
	Screen *screen;
	
	screen = XDefaultScreenOfDisplay(display);
	return(XDefaultColormapOfScreen(screen));
}


void connectionSetup()
{
xConnClientPrefix clientPrefix;

struct
  {
  xConnSetupPrefix hdr;
  xConnSetup       setup;
  char             vendor[32];
  xPixmapFormat    format1;
  xPixmapFormat    format8;
  xWindowRoot      window;
  xDepth           depth1;
  xDepth           depth8;
  xVisualType      visual8;
  } reply;

std::cerr << "Connection Setup Read"<< std::endl;
Xwindow::X11sock -> read((char *) &clientPrefix,sizeof(xConnClientPrefix));
byteOrder = clientPrefix.byteOrder;
std::cerr << "Byte Order of Client : " << byteOrder << std::endl;
// Think I need to set byteorder on X11 socket
std::memset(&reply,0,sizeof(reply));
reply.hdr.success = 1;
reply.hdr.majorVersion = 11;
reply.hdr.minorVersion = 0;
//reply.hdr.length = (sizeof(reply) - 32) >> 2;
reply.hdr.length = 40;
reply.setup.release = 11;
reply.setup.ridBase = 0x05000000;
reply.setup.ridMask = 0x003FFFFF;
reply.setup.nbytesVendor = 0x1E;
reply.setup.maxRequestSize = 1024;
reply.setup.numRoots = 1;
reply.setup.numFormats = 2;
reply.setup.imageByteOrder = MSBFirst;
reply.setup.bitmapBitOrder = MSBFirst;
reply.setup.bitmapScanlineUnit = 8;
reply.setup.bitmapScanlinePad = 8;
reply.setup.minKeyCode = FIRSTKEY;
reply.setup.maxKeyCode = 141;
reply.setup.pad2 = 0;
strcpy(reply.vendor,"JavaXServ");
reply.format1.depth = 1;
reply.format1.bitsPerPixel = 1;
reply.format1.scanLinePad = 8;          // Trace is in hex
reply.format8.depth = 8;
reply.format8.bitsPerPixel = 8;
reply.format8.scanLinePad  = 8;
reply.window.windowId = rootWin -> X11windowId();
// display is a global variable
reply.window.defaultColormap = initialColourMap(); // using default colour map from X11 where JavaXServ is running
reply.window.whitePixel = 0x0;
reply.window.currentInputMask = 0x4180003F;
reply.window.pixWidth = 600;
reply.window.pixHeight = 800;
reply.window.mmWidth = 156;
reply.window.mmHeight = 112;
reply.window.minInstalledMaps = 1;
reply.window.maxInstalledMaps = 1;
reply.window.rootVisualID = 0x20;
reply.window.rootDepth = 8;
reply.window.nDepths = 2;
reply.depth1.depth = 1;
reply.depth1.nVisuals = 0;
reply.depth8.depth = 8;
reply.depth8.nVisuals = 1;
reply.visual8.visualID = 0x20;
reply.visual8.c_class = StaticColor;  // Need to support 8 bit Pseudocolour for Old Applications
reply.visual8.bitsPerRGB = 8;
reply.visual8.colormapEntries = 256;
reply.visual8.redMask = 0xFF;
reply.visual8.greenMask = 0xFF00;
reply.visual8.blueMask = 0xFF0000;
// Need to add support for 24bit true colour for new applications
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
std::cerr << "Reply Authourized" << std::endl;
}

void internAtom(xInternAtomReq *ptr)
{
xInternAtomReply reply;
char *namePtr= ((char *) ptr) + sizeof(xInternAtomReq);
char name[120];

::memcpy(name,namePtr,ptr -> nbytes);
name[ptr -> nbytes] = '\0';

std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
std::cerr << "Reply sequence Number : " << sequenceNum << std::endl;
reply.atom = XInternAtom(display,name, ptr -> onlyIfExists);
std::cerr << "InternAtom : " << name << " Value : " << reply.atom << std::endl;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
}

void getAtomName(xResourceReq *ptr)
{
xGetAtomNameReply reply;
char *namePtr;
int len;

std::cerr << "getAtomName" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
namePtr = XGetAtomName(display,ptr -> id);
len     = strlen(namePtr);
reply.length = (len + 3) >> 2;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
Xwindow::X11sock -> write(namePtr,(reply.length << 2));
}

void replyNoProperty(void)
{
xGetPropertyReply reply;

std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.format = 0;
reply.sequenceNumber = sequenceNum;
reply.length = 0;
reply.propertyType = 0;
reply.bytesAfter = 0;
reply.nItems = 0;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
}

//
// Need to change this to support property properly
// Making XGetWindowPropert on javaServ DISPLAY will
// get found out with some applications
//
void getProperty(xGetPropertyReq *ptr)
{
Atom actual;
unsigned long items,bytes;
unsigned char *prop;
int format,ret,len;

xGetPropertyReply reply;

std::cerr << "GetProperty " << ptr -> property << " type "
                       << ptr -> type << std::endl;
if ( ptr -> window == rootWin -> X11windowId())
   {
   std::cerr << "Root window XGetWindowProperty : " << std::endl;
   ret = XGetWindowProperty(display,ptr -> window,ptr -> property,
         ptr -> longOffset, ptr -> longLength,ptr -> c_delete,
         ptr -> type,&actual,&format,&items,&bytes,&prop);
   if ( ret != 0 )
      {
      std::cerr << "XGetWindowProperty failed : " << ret << std::endl;
      }
   else
      {
      // Okay success now need to Free
      std::cerr << "Xfree " << std::endl; 
      XFree(prop);
      }
   if ( bytes > 0 )
      std::cerr << "Only Partial Read of Property need to code for" << std::endl;

  std::cerr << "actual : " << actual << " format : " << format << " items : " << items << std::endl;
  }
else
  {
  std::cerr << "Non Root Window " << std::endl;
  actual = 0;
  items = format = 0;
  bytes = 0;
  }
std::cerr << "Send Reply to GetProperty" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.format = format;
reply.sequenceNumber = sequenceNum;
std::cerr << "Sequence Number : " << sequenceNum << std::endl;
switch ( format )
   {
   case 0  :
        reply.length = 0;
        break;

   case 8  :
        reply.length = (items + 3) >> 2;
        len = items + bytes;
        break;

   case 16 :
        reply.length = (items + 1) >> 1;
        len = (items << 1) + bytes;
        break;

   case 32 :
        reply.length = items;
        len = (items << 2) + bytes;
        break;

   default :
        std::cerr << "Type not yet coded for : guess bytes" << std::endl;
        reply.length = (items + 3) >> 2;
        len = items + bytes;
        break;
   }
reply.propertyType = actual;
reply.bytesAfter = bytes;
reply.nItems = items;
// dumpHex((char *) &reply,32);
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
if ( actual != 0 )
   {  
   Xwindow::X11sock -> write((char *) prop,len);
   Xwindow::X11sock -> write("    ",(reply.length << 2) - len);
   }
// Call XFree to free results ?
}

int fontPointSize(char *str)
{
// Don't seem to be able to use sscanf for this
int l = strlen(str);
int i,j,ps;

i = j = 0;
while ( j < 8 && i < l)
   {
   if (*str == '-' ) j++;
   str++;
   i++;
   }
if ( ::sscanf(str,"%n-",&ps) != 1 )
   {
   std::cerr << "Error with Font Point size" << std::endl;
   }
std::cerr << "Point size : " << ps << std::endl;
return(ps);
}

void queryFont(xResourceReq *ptr)
{
XFontStruct *fontStr;
xQueryFontReply reply;
int i;

std::cerr << "New Query Font fid : " << ptr -> id;
// Find font in our table
i = 0;
while ( i < fontEntries && fontTable[i].fid != ptr -> id)
   {
   i++;
   }
if ( i == fontEntries )
   {
   std::cerr << "fid " << ptr -> id << " Not in our Table" << std::endl;
   exit(1);
   }
std::cerr << " FONT " << fontTable[i].font << std::endl;
if ((fontStr = ::XQueryFont(display,fontTable[i].font)) == NULL )
   {
   std::cerr << "Font not found" << std::endl;
   exit(1);
   }
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.pad1 = 0;
reply.sequenceNumber = sequenceNum;
::memcpy(&reply.minBounds,&(fontStr -> min_bounds),12);
::memcpy(&reply.maxBounds,&(fontStr -> max_bounds),12);
reply.minCharOrByte2 = fontStr -> min_char_or_byte2;
reply.maxCharOrByte2 = fontStr -> max_char_or_byte2;
reply.defaultChar = fontStr -> default_char;
reply.nFontProps = fontStr -> n_properties;
reply.drawDirection = fontStr -> direction;
reply.minByte1 = fontStr -> min_byte1;
reply.maxByte1 = fontStr -> max_byte1;
reply.allCharsExist = fontStr -> all_chars_exist;
reply.fontAscent = fontStr -> ascent;
reply.fontDescent = fontStr -> descent;
reply.nCharInfos = reply.maxCharOrByte2 - reply.minCharOrByte2 + 1;
std::cerr << "Number of Chars : " << reply.nCharInfos << std::endl;
std::cerr << "Number of properties : " << reply.nFontProps << std::endl;
std::cerr << "Size of reply : " << sizeof(reply) / 4 << std::endl;
reply.length = (sizeof(reply)/4) - 8 + (2 * reply.nFontProps) + ( 3 * reply.nCharInfos); // Length is additional length so miuns 8 byte header
std::cerr << "Reply Length : "<< reply.length << std::endl;
Xwindow::X11sock -> assert();
Xwindow::X11sock -> put((char *) &reply,sizeof(reply));
Xwindow::X11sock -> put((char *) fontStr -> properties,8 * reply.nFontProps);
Xwindow::X11sock -> put((char *) fontStr -> per_char,12 * reply.nCharInfos);
Xwindow::X11sock -> flush();
// ::XFreeFontInfo(display,fontStr,1);
}


void listFonts(xListFontsReq *ptr)
{
// Must be multiple of 4 bytes

xListFontsReply reply;
char **listPtr;
char **wrkptr;
char pattern[MAXFONTNAME];
int num;
int i;
int sl;
char cl;

std::cerr << "List Fonts" << std::endl;
if ( strlen((char *) ptr + sizeof(xListFontsReq)) != ptr -> nbytes)
   {
   std::cerr << " Need to copy string" << std::endl;
   }
strncpy(pattern,(char *) ptr + sizeof(xListFontsReq),ptr -> nbytes);
pattern[ptr -> nbytes] = NULL;

listPtr = ::XListFonts(display,pattern,ptr -> maxNames,&num);
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
reply.nFonts = num;
sl = 0;
wrkptr = listPtr;
for ( i=0; i < num; i++)
    {
    sl += strlen(*wrkptr) + 1;  // string preceded by 1 byte length
    wrkptr++;
    }
reply.length = (sl + 3) >> 2;
std::cerr << "Fonts : " << num << " Total String Length " << sl << std::endl;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
wrkptr = listPtr;
for ( i=0; i < num; i++)
    {
    std::cerr << " Font : " << (*wrkptr) << " strlen : " << strlen(*wrkptr);
    cl = (char) strlen(*wrkptr);
    Xwindow::X11sock -> write(&cl,1);
    Xwindow::X11sock -> write(*wrkptr,strlen(*wrkptr));
    wrkptr++;
    }
Xwindow::X11sock -> write("    ",(reply.length << 2) - sl);
::XFreeFontNames(listPtr);
}

void openFont(xOpenFontReq *ptr)
{
char name[MAXFONTNAME];

strncpy(name,(char *) ptr + sizeof(xOpenFontReq),ptr -> nbytes);
name[ptr -> nbytes] = NULL;
std::cerr << "Open Font : " << name << " Fid " << ptr -> fid;

if ( fontEntries < MAXFONTS )
   {
   if ((fontTable[fontEntries].font = ::XLoadFont(display,name)) == NULL )
      {
      std::cerr << "Open Font failed" << std::endl;
      }
   else
      {
      fontTable[fontEntries].pointSize = fontPointSize(name);
      fontTable[fontEntries++].fid     = ptr -> fid;
      }
   }
else
   {
   std::cerr << "Out of Font Table entries" << std::endl;
   }
}

void  listFontsWithInfo(xListFontsReq *ptr)
{
xListFontsWithInfoReply reply;
XFontStruct *fontPtr;
XFontStruct *wrkptr;
char        **namePtr;
char        pattern[MAXFONTNAME];
int i, num, sl,pl;

std::cerr << "List Fonts with Info" << std::endl;
strncpy(pattern,(char *) ptr + sizeof(xListFontsReq),ptr -> nbytes);
pattern[ptr -> nbytes] = NULL;

std::cerr << pattern << std::endl;

namePtr = ::XListFontsWithInfo(display,pattern,ptr -> maxNames,&num,&fontPtr);
std::memset(&reply,0,sizeof(reply));

std::cerr << "Number : " << num << std::endl;
reply.type = 1;
reply.sequenceNumber = sequenceNum;
wrkptr = fontPtr;
for ( i=0; i < num; i++)
    {
    reply.nameLength = sl = strlen(*namePtr);
    ::memcpy(&reply.minBounds,&(wrkptr -> min_bounds),12);
    ::memcpy(&reply.maxBounds,&(wrkptr -> max_bounds),12);
    reply.minCharOrByte2 = wrkptr -> min_char_or_byte2;
    reply.maxCharOrByte2 = wrkptr -> max_char_or_byte2;
    reply.defaultChar = wrkptr -> default_char;
    reply.nFontProps = wrkptr -> n_properties;
    reply.drawDirection = wrkptr -> direction;
    reply.minByte1 =wrkptr -> min_byte1;
    reply.maxByte1 = wrkptr -> max_byte1;
    reply.allCharsExist = wrkptr -> all_chars_exist;
    reply.fontAscent = wrkptr -> ascent;
    reply.fontDescent = wrkptr -> descent;
    std::cerr << "Number of properties : " << reply.nFontProps << std::endl;
    reply.length = (sizeof(reply) / 4) - 8 + (2 * reply.nFontProps) + ((sl + 3) >> 2); // Additional length i.e minus 8 byte header
    std::cerr << "Font string length " << sl;
    std::cerr << " rounded " << ((sl + 3) >> 2) << std::endl;
    Xwindow::X11sock -> assert();
    Xwindow::X11sock -> put((char *) &reply,sizeof(reply));
    Xwindow::X11sock -> put((char *) wrkptr -> properties,(pl = (8 * reply.nFontProps)));
//    Xwindow::X11sock -> put(*namePtr++,sl);
    Xwindow::X11sock -> put(*namePtr,sl);
    Xwindow::X11sock -> put("    ",( 4 - (sl % 4 )) % 4);
    Xwindow::X11sock -> flush();
    wrkptr++;
    }
// Last in series
reply.nameLength = 0;
reply.length = 7;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));

// Comment out for now
//::XFreeFontInfo(namePtr,fontPtr,num);
}

int checkJavaWid(int wid)
{
int jid;
Xwindow *ptr = rootWin -> AddressWin(wid);

if ((jid = ptr -> JavaWid(wid)) == -1 )
   {
   jid = ptr -> CreateJavaWin();
   }
return(jid);
}

void getFontPath(xResourceReq *ptr)
{
xGetFontPathReply reply;
char **pathPtr,**wrkPtr;
int  i,num,l,nl;

std::cerr << "Get Font Path" << std::endl;
reply.type = 1;
reply.sequenceNumber = sequenceNum;
pathPtr = XGetFontPath(display,&num);
reply.nPaths = num;
nl = 0;
wrkPtr = pathPtr;
for ( i = 0; i < num; i++)
    {
    nl += ( 1 + strlen(*wrkPtr++));
    }
reply.length = (nl + 3)/4;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
Xwindow::X11sock -> assert();
wrkPtr = pathPtr;
for ( i = 0; i < num; i++)
    {
    Xwindow::X11sock -> putByte(l = strlen(*wrkPtr));
    Xwindow::X11sock -> put(*wrkPtr++,l);
    }
i = 0;
Xwindow::X11sock -> put((char *) &i,(reply.length << 2) - nl);
Xwindow::X11sock -> flush();
XFreeFontPath(pathPtr);
}

void changeProperty(xChangePropertyReq *ptr)
{
char *namePtr;
// Should check if application wants event
// Should also deal with actual change

namePtr = XGetAtomName(display,ptr -> property);
std::cerr << "Change Property : " << namePtr << std::endl;

#ifdef OLD
xEvent event;

std::cerr << "Change Property Event" << std::endl;
std::memset(&event,0,sizeof(event));
event.u.u.type = 28;
event.u.u.sequenceNumber = sequenceNum;
event.u.property.window = ptr -> window;
event.u.property.atom = ptr -> property;
event.u.property.time = clock();
event.u.property.state = 0;             // Change Value
Xwindow::X11sock -> write((char *) &event,sizeof(event));
std::cerr << "Send Change Property Event" << std::endl;
#endif
}

void convertSelection(xConvertSelectionReq *ptr)
{
xEvent event;

std::cerr << "Selection Notify Event" << std::endl;
std::memset(&event,0,sizeof(event));
event.u.u.type = 31;
event.u.u.sequenceNumber = sequenceNum;
event.u.selectionNotify.time = 0;       // Current Time
event.u.selectionNotify.requestor = ptr -> requestor;
event.u.selectionNotify.selection = ptr -> selection;
event.u.selectionNotify.target = ptr -> target;
event.u.selectionNotify.property = ptr -> property;
Xwindow::X11sock -> write((char *) &event,sizeof(event));
std::cerr << "Send Change Property Event" << std::endl;
}

void mapWindow(xResourceReq *ptr)
{
int pstate;
Xwindow *winptr = rootWin -> AddressWin(ptr -> id);

std::cerr << "Map Window : " << ptr -> id << std::endl;
if ( winptr != NULL )
   {
   std::cerr << "Get ParentMapStat" << std::endl;
   pstate = winptr -> ParentMapState();
   std::cerr << "Now Map the Window parent state :" << pstate << std::endl;
   winptr -> MapWindow(pstate,sequenceNum);
   if ( pstate == 2 )
      winptr -> exposePendingSubWindows(sequenceNum);
   }
}

void unMapWindow(xResourceReq *ptr)
{
Xwindow *winptr = rootWin -> AddressWin(ptr -> id);
winptr -> UnMapWindow();
}

void mapSubwindows(xResourceReq *ptr)
{
Xwindow *winptr = rootWin -> AddressWin(ptr -> id);

std::cerr << "Map SubWindows" << std::endl;
if ( winptr != NULL )
   winptr -> MapSubWindows(sequenceNum);
}


void queryExtension(xQueryExtensionReq *ptr)
{
xQueryExtensionReply reply;
char extension[120];

// Get extension
std::memset(extension,0,sizeof(extension));
::strncpy(extension,(char *) ptr + sizeof(xQueryExtensionReq),ptr -> nbytes);

std::cerr << "Query Extension : " << extension << std::endl;

// Reply Not supported
std::cerr << "****** Reply : Not supported" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
// None  present = 0;
reply.present = 0;
reply.sequenceNumber = sequenceNum;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
}

void getSelectionOwner(xResourceReq *ptr)
{
xGetSelectionOwnerReply reply;

std::cerr << "Get Selection Owener Reply " << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
}

void queryColors(xQueryColorsReq *ptr)
{
xQueryColorsReply reply;
xrgb rgb;

std::cerr << "Query Colors" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
reply.length = 4;
// For now just do as Xcalc trace
reply.nColors = 2;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));

rgb.red   = 0;
rgb.blue  = 0;
rgb.green = 0;
Xwindow::X11sock -> write((char *) &rgb,sizeof(rgb));
rgb.red   = 0xFF00;
rgb.blue  = 0xFF00;
rgb.green = 0xFF00;
Xwindow::X11sock -> write((char *) &rgb,sizeof(rgb));
}

void allocColor(xAllocColorReq *ptr)
{
    Status status;
    XColor xcolour;
    xAllocColorReply reply;
    	
    std::cerr << "AllocColor - ColorMap :" << ptr -> cmap
          << " Red   : " << ptr -> red
          << " Green : " << ptr -> green
          << " Blue  : " << ptr -> blue
          << std::endl;
          
    // Do the actual Alloc at the Server
    std::memset(&xcolour,0,sizeof(xcolour));
    xcolour.red   = ptr -> red;
    xcolour.green = ptr -> green;
    xcolour.blue  = ptr -> blue;
    status = XAllocColor(display,ptr -> cmap,&xcolour);          
    if (status == 0 )
       std::cerr << "XAllocColor failed" << std::endl;
       
    // Send the Reply
    std::memset(&reply,0,sizeof(reply));
    reply.type = 1;
    reply.sequenceNumber = sequenceNum;
    reply.length = 0;
    reply.red    = xcolour.red;
    reply.green  = xcolour.green;
    reply.blue   = xcolour.blue;
    reply.pixel  = xcolour.pixel;
    Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
    
    // Add code to send to java if not a direct colour     
}
void allocNamedColor(xAllocNamedColorReq *ptr)
{
    Status status;
    XColor screenColour;
    XColor exactColour; 
    char name[MAXFONTNAME];
    xAllocNamedColorReply reply;    
    ::strncpy(name,(char *) ptr + sizeof(xAllocNamedColorReq),ptr -> nbytes);
    name[ptr -> nbytes] = NULL;
    std::cerr << "AllocNamedColor : " << name << " ColorMap :" << ptr -> cmap << std::endl;
    // Do the actual AllocName at the Server
    std::memset(&screenColour,0,sizeof(screenColour));
    std::memset(&exactColour,0,sizeof(exactColour));
    status = XAllocNamedColor(display,ptr -> cmap,name,&screenColour,&exactColour);          
    if (status == 0 )
       std::cerr << "XAllocNamedColor failed" << std::endl;
       
    // Send the Reply
    std::memset(&reply,0,sizeof(reply));
    reply.type = 1;
    reply.sequenceNumber = sequenceNum;
    reply.length 	 = 0;
    reply.pixel       = exactColour.pixel;
    reply.exactRed    = exactColour.red;
    reply.exactGreen  = exactColour.green;
    reply.exactBlue   = exactColour.blue;
    reply.screenRed   = screenColour.red;
    reply.screenGreen = screenColour.green;
    reply.screenBlue  = screenColour.blue;
    Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
    
    // Add code to send to java if not a direct colour     
}

void lookupColor(xLookupColorReq *ptr)
{
    Status status;
    xLookupColorReply reply;
    XColor returnColour,screenColour;
    char name[MAXFONTNAME];
    ::strncpy(name,(char *) ptr + sizeof(xLookupColorReq),ptr -> nbytes);
    name[ptr -> nbytes] = NULL;
    std::cerr << "LookupColor : " << name << " ColorMap :" << ptr -> cmap << std::endl;

    status = XLookupColor(display,ptr -> cmap,name,&returnColour,&screenColour);          
    if (status == 0 )
       std::cerr << "XLookupColor failed" << std::endl;

    // Send the Reply
    std::memset(&reply,0,sizeof(reply));
    reply.type = 1;
    reply.sequenceNumber = sequenceNum;
    reply.length 	 = 0;
    reply.exactRed    = returnColour.red;
    reply.exactGreen  = returnColour.green;
    reply.exactBlue   = returnColour.blue;
    reply.screenRed   = screenColour.red;
    reply.screenGreen = screenColour.green;
    reply.screenBlue  = screenColour.blue;
    Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
    // Add code to send to java if not a direct colour  
}

void createWindow(xCreateWindowReq *ptr)
{
Xwindow *winptr;
int     i,mask;
std::cerr << "Create X11 window : " << ptr -> wid
          << " parent : " << ptr -> parent
          << std::endl;
// ::printf("WID(hex) : %x Parent %x \n",ptr -> wid,ptr -> parent);
mask = getEventMask(ptr -> mask,((int *) ptr ) + 8);
if ((mask & ExposureMask ) == ExposureMask)
   std::cerr << "Exposure Mask Set" << std::endl;
if ((winptr = rootWin -> AddressWin(ptr -> parent)) != 0 )
   {
   (winptr -> CreateSubWindow(ptr -> wid,mask,ptr -> x,ptr -> y,
                           ptr -> width,ptr -> height,ptr -> depth,
                           ptr -> borderWidth)) -> CreateJavaWin();

   }
else
   {
   std::cerr << "Error Parent Window " << ptr -> parent << "does not exist" << std::endl;
   }
}

void configWindow(xConfigureWindowReq *ptr)
{
Xwindow *winPtr = rootWin -> AddressWin(ptr -> window);
int id = winPtr -> JavaWid(ptr -> window);

std::cerr << "Config Window" << std::endl;
winPtr -> ConfigWindow(ptr -> mask,((int *) ptr) + 3);

// If already created in java forward with ID changed
if ( id != -1 )
   {
   ptr -> window = id;
   Xwindow::javasock -> write((char *) ptr,(ptr -> length)  << 2);
   }
}

void createPixmap(xCreatePixmapReq *ptr)
{
Xwindow *pm;

std::cerr << "Create PixMap : " << ptr-> pid <<
             " Width : " << ptr-> width <<
             " Height : " << ptr-> height <<
             " Depth : " << ptr-> depth << std::endl;
// Create Pixmap contol on server
pm = rootWin -> CreatePixmap(ptr-> pid,ptr -> width,ptr -> height,ptr -> depth);
// Create Pixmap on Java Client
std::cerr << "Create Java Pixmap : "<< pm << std::endl;
pm -> DumpWindow();
pm -> CreateJavaPixmap();
}

void freePixmap(xResourceReq *ptr)
{
std::cerr << "Free PixMap : " << ptr-> id << std::endl;
}

void changeWindowAttributes(xChangeWindowAttributesReq *ptr)
{
Xwindow *winPtr;
int     mask;
//
//      Send Config Notify Event
//
std::cerr << "Change Window Attributes" << std::endl;
winPtr = rootWin -> AddressWin(ptr -> window);
mask   = getEventMask(ptr -> valueMask,((int *) ptr) + 3);
winPtr -> ConfigNotify(mask,sequenceNum);
//
//      Note More events than this were generated for xclock
//

//
//      Send Expose Event
//
if (( mask & ExposureMask ) == ExposureMask )
   {
   std::cerr << "Change attribute to expose" << std::endl;
   winPtr -> expose(sequenceNum);
   winPtr -> exposePendingSubWindows(sequenceNum);
   }
}

void getWindowAttributes(xResourceReq *ptr)
{
Xwindow *winPtr;

std::cerr << "Get Window Attributes" << std::endl;
winPtr = rootWin -> AddressWin(ptr -> id);
winPtr -> GetWindowAttributes(sequenceNum);
}

void destroyWindow(xResourceReq *ptr)
{
xEvent event;

std::cerr << "Destroy Notify Event" << std::endl;
std::memset(&event,0,sizeof(event));
event.u.u.type = 28;
event.u.u.sequenceNumber = sequenceNum;
event.u.destroyNotify.event = ptr -> id;
event.u.destroyNotify.window = ptr -> id;
Xwindow::X11sock -> write((char *) &event,sizeof(event));
std::cerr << "Send Destroy Notify Event" << std::endl;

// Convert window Id from X11 to java Id & forward request.
if (( ptr -> id = rootWin -> JavaWid(ptr -> id)) != -1 )
   Xwindow::javasock -> write((char *) ptr,(ptr -> length)  << 2);
}

void getGeometry(xResourceReq *ptr)
{
Xwindow *winptr = rootWin -> AddressWin(ptr -> id);

std::cerr << "Get Geometry" << std::endl;
if ( winptr != NULL )
   winptr -> GetGeometry(rootWin -> X11windowId(0),sequenceNum);
}

void queryTree(xResourceReq *ptr)
{
Xwindow *winptr = rootWin -> AddressWin(ptr -> id);

std::cerr << "Query Tree" << std::endl;
if ( winptr != NULL )
   winptr -> QueryTree(rootWin -> X11windowId(0),sequenceNum);
}


void clearArea(xClearAreaReq *ptr)
{
Xwindow *winPtr = rootWin -> AddressWin(ptr -> window);

std::cerr << "Clear Area Routine : " << (int) (ptr -> exposures) << " :" << std::endl;
if ( ptr -> exposures == (int) 1 )
   {
   std::cerr << "Clear Area Expose" << std::endl;
   winPtr -> exposeMust(sequenceNum,ptr -> x,ptr -> y,
                                    ptr -> width, ptr -> height);
   }
// Convert window Id from X11 to java Id & forward request.
if (( ptr -> window = winPtr -> JavaWid(ptr -> window)) != -1 )
   {
   Xwindow::javasock -> write((char *) ptr,(ptr -> length)  << 2);
   }
else
   {
   std::cerr << "Clear Area for Non exposed Area" << std::endl;
   }
}

void getKeyboardMapping(xGetKeyboardMappingReq *ptr)
{
// May not have to do all this, just match java to mapping
// Starts at 9, for 100, 3 per key
// keymap in include keymap.c

xGetKeyboardMappingReply reply;

std::cerr << "Get Keyboard Mapping" << std::endl;
std::memset(&reply,0,sizeof(reply));
if ( ptr -> count <= 133 )
   {
   reply.type = 1;
   reply.keySymsPerKeyCode = 3;
   reply.sequenceNumber = sequenceNum;
   reply.length = 3 * (ptr -> count);
   Xwindow::X11sock -> write((char *) &reply,32);
   Xwindow::X11sock -> write((char *) &keymap[(ptr -> firstKeyCode) - FIRSTKEY],
                    12 * (ptr -> count));
   }
else
   {
   std::cerr << "Count : " << ptr -> count << " larger than keymap table " <<std::endl;
   }
}

void getKeyboardControl(xResourceReq *ptr)
{
xGetKeyboardControlReply reply;

std::cerr << "Get Keyboard Control" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.globalAutoRepeat = 0;
reply.sequenceNumber = sequenceNum;
reply.length = 5;
reply.ledMask = 0;
reply.keyClickPercent = 0;
reply.bellPercent = 0x32;
reply.bellPitch = 0x190;
reply.bellDuration = 0x64;
Xwindow::X11sock -> write((char *) &reply,sizeof(reply));
}

void getModifierMapping(xResourceReq *ptr)
{
xGetModifierMappingReply reply;

char mods[16] = { 0x34, 0x41, 0x26, 0x00, 0x42, 0x48, 0x44, 0x00,
                  0x46, 0x8d, 0x00, 0x00, 0x00, 0x00, 0x62, 0x00 };

std::cerr << "Get Modifier Mapping" << std::endl;
std::memset((char *) &reply,0,sizeof(reply));
reply.type = 1;
reply.numKeyPerModifier = 2;
reply.sequenceNumber = sequenceNum;
reply.length = 4;
Xwindow::X11sock -> write((char *) &reply,32);
Xwindow::X11sock -> write((char *) &mods,16);
}

void grabKeyboard(xGrabKeyboardReq *ptr)
{
xGrabKeyboardReply reply;

std::cerr << "Grab Keyboard" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.status = 0;
reply.sequenceNumber = sequenceNum;
reply.length = 0;
Xwindow::X11sock -> write((char *) &reply,32);
}

void getInputFocus()
{
xGetInputFocusReply reply;

std::cerr << "Get Input Focus" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
reply.length = 0;
Xwindow::X11sock -> write((char *) &reply,32);
}

void getScreenSaver(xResourceReq *ptr)
{
xGetScreenSaverReply reply;

std::cerr << "Get Screen Saver" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
reply.length = 0;
Xwindow::X11sock -> write((char *) &reply,32);
}

void getPointerControl(xResourceReq *ptr)
{
xGetPointerControlReply reply;

std::cerr << "Get Pointer Control" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
reply.length = 0;
reply.accelNumerator = 0x02;
reply.accelDenominator = 0x01;
reply.threshold = 0x04;
Xwindow::X11sock -> write((char *) &reply,32);
}

void queryBestSize(xQueryBestSizeReq *ptr)
{
xQueryBestSizeReply reply;

std::cerr << "Query Best Size" << std::endl;
std::memset(&reply,0,sizeof(reply));
reply.type = 1;
reply.sequenceNumber = sequenceNum;
reply.length = 0;
reply.width = ptr -> width;
reply.height = ptr -> height;
Xwindow::X11sock -> write((char *) &reply,32);
}

void reportRequest(int type)
{
	switch (type)
	{
	case X_ConvertSelection :
           std::cerr << "X_ConvertSelection" << std::endl;
           break;

      case X_InternAtom :
      	   std::cerr << "X_InternAtom" << std::endl;
           break;

      case X_GetAtomName :
           std::cerr << "X_GetAtomName" << std::endl;
           break;

      case X_QueryFont :
           std::cerr << "X_QueryFont" << std::endl;
           break;

      case X_OpenFont :
      	   std::cerr << "X_OpenFont" << std::endl;
           break;

      case X_ListFonts :
           std::cerr << "X_ListFonts" << std::endl;
           break;

      case X_GetFontPath :
           std::cerr << "X_GetFontPath" << std::endl;
           break;

      case X_ListFontsWithInfo :
           std::cerr << "X_ListFontsWithInfo" << std::endl;
           break;

      case X_ChangeProperty :
           std::cerr << "X_ChangeProperty" << std::endl;
           break;

      case X_MapWindow :
           std::cerr << "X_MapWindow" << std::endl;
           break;

      case X_MapSubwindows :
           std::cerr << "X_MapSubwindows" << std::endl;
           break;

      case X_UnmapWindow :
           std::cerr << "X_UnmapWindow" << std::endl;
           break;

      case X_ChangeWindowAttributes :
           std::cerr << "X_ChangeWindowAttributes" << std::endl;
           break;

      case X_GetWindowAttributes :
           std::cerr << "X_GetwindowAttributes" << std::endl;
           break;

      case X_GetGeometry :
      	   std::cerr << "X_GetGeometry" << std::endl;
           break;

      case X_QueryTree :
           std::cerr << "X_QueryTree" << std::endl;
           break;

      case X_GetProperty :
           std::cerr << "X_GetProperty" << std::endl;
           break;

      case X_QueryExtension :
           std::cerr << "X_QueryExtension" << std::endl;
           break;

      case X_QueryColors :
	   std::cerr << "X_QueryColors" << std::endl;
	   break;

      case X_CreatePixmap :
           std::cerr << "X_CreatePixMap" << std::endl;
           break;
           
      case X_FreePixmap :
      	   std::cerr << "X_FreePixmap" << std::endl;
      	   break;
      	       
      case X_GetSelectionOwner :
           std::cerr << "X_GetSelectionOwner" << std::endl;
           break;

      case X_GrabKeyboard :
           std::cerr << "X_GrabKeyboard" << std::endl;
           break;

      case X_GetKeyboardControl :
           std::cerr << "X_GetKyboardControl" << std::endl;
           break;

      case X_GetKeyboardMapping :
           std::cerr << "X_GetKeyboardMapping" << std::endl;
           break;

      case X_GetModifierMapping :
           std::cerr << "X_GetModiiferMapping" << std::endl;
           break;

      case X_GetPointerControl :
           std::cerr << "X_GetPointerControl" << std::endl;
           break;

      case X_GetScreenSaver :
           std::cerr << "X_GetScreenSaver" << std::endl;
           break;

      case X_QueryBestSize :
           std::cerr << "X_QueryBestSize" << std::endl;
           break;

      case X_CreateWindow :
           std::cerr << "X_CreateWindow" << std::endl;
           break;

      case X_ConfigureWindow :
           std::cerr << "X_ConfigureWindow" << std::endl;
           break;

      case X_DestroyWindow :
           std::cerr << "X_DestroyWindow" << std::endl;
           break;

      case X_ClearArea :
           std::cerr << "Clear Area" << std::endl;
           break;

      case X_AllocNamedColor :
      	   std::cerr << "X_AllocNamedColor" << std::endl;
      	   break;
      	   
      case X_AllocColor :
      	   std::cerr << "X_AllocColor" << std::endl;
      	   break;
      	   
      case X_GetInputFocus :
      	   std::cerr << "X_GetInputFocus" << std::endl;
      	   break;
      	   
      case X_PolyPoint :
           std::cerr << "X_PolyPoint" << std::endl;
           break;
           
      case X_PolyLine :
           std::cerr << "X_PolyLine" << std::endl;
           break;
           
      case X_PolySegment :
           std::cerr << "X_PolySegment" << std::endl;
           break;
           
      case X_PolyRectangle :
           std::cerr << "X_PolyRectangle" << std::endl;
           break;
           
      case X_PolyArc :
           std::cerr << "X_PolyArc" << std::endl;
           break;
           
      case X_FillPoly :
           std::cerr << "X_FillPoly" << std::endl;
           break;
           
      case X_PolyFillRectangle :
           std::cerr << "X_PolyFillRectangle" << std::endl;
           break;
           
      case X_PolyFillArc :
           std::cerr << "X_PlyFillArc" << std::endl;
           break;
           
      case X_PolyText8 :
           std::cerr << "X_PolyText8" << std::endl;
           break;
           
      case X_PutImage :
           std::cerr << "X_PutImage" << std::endl;
           break;
           
      case X_SetClipRectangles :
           std::cerr << "X_SetClipRectangles" << std::endl;
           break;

      case X_CopyPlane :
      	   std::cerr << "X_CopyPlane" << std::endl;
      	   break;
      	   
      case X_CopyArea :
           std::cerr << "X_CopyArea" << std::endl;
           break;
 
      case X_CreateGC :
           std::cerr << "X_Create GC" << std::endl;
		   break;
			      
      case X_FreeGC :
      	   std::cerr << "X_FreeGC" << std::endl;
      	   break;
      	   
      case X_CreateGlyphCursor :
           std::cerr << "X_CreateGlyphCursor" << std::endl;
           break;
           
      case X_ChangeGC :
           std::cerr << "X_ChangeGC" << std::endl;
           break;

      case X_LookupColor :
	   std::cerr << "X_LookupColor" << std::endl;
	   break;
           
      case 129 :     // Shape extension
           std::cerr << "X_ShapeExtension" << std::endl;
           break;

      default :
           std::cerr << "Not yet reported : " << type << std::endl;
		   break;	
	}
	
}

int processRequests(void)
{
int len,hl,ret;
struct
  {
  xResourceReq req;
  int data[256];
  } requestBuff;

std::cerr << "Process X11 Request" << std::endl;
len = 4;
while ( len > 0 )       // Need to change to while bytes available
   {
   // Read in Header
   if (( hl = Xwindow::X11sock -> readBlock((char *) &requestBuff.req,4)) != 4)
      {
      if ( hl == 0 )
         {
         std::cerr << "End of X11 Stream" << std::endl;
         return(0);
         }
      else
         {
         std::cerr << "Error reading Header : " << hl << " Bytes Read " << std::endl;
         return(-2);
         }
      }
   // Get remaining length
   len = ( requestBuff.req.length - 1) << 2;
   if ( len > 1024 )
      {
      std::cerr << "X11 Request data > 1024 bytes" << std::endl;
      return(-2);
      }
   if (( ret = Xwindow::X11sock -> readBlock((char *) &requestBuff.req.id,len)) != len )
      {
      if ( ret == 0 )
	 	 {
         std::cerr << "End of X11 Stream" << std::endl;
         return(0);
	     }
      else
	     {
         std::cerr << "Error reading Request data" << std::endl;
         return(-2);
	     }
      }
   len += 4;
   sequenceNum++;
   std::cerr << "Request Type : " << (int) requestBuff.req.reqType <<
   " Sequence Number : " << sequenceNum << std::endl;
   reportRequest( requestBuff.req.reqType );
   switch ( requestBuff.req.reqType )
      {
      //************************************************//
      //  Deal with locally                             //
      //************************************************//
      case X_ConvertSelection :
           convertSelection((xConvertSelectionReq *) &requestBuff);
           break;

      case X_InternAtom :
           internAtom((xInternAtomReq *) &requestBuff);
           break;

      case X_GetAtomName :
           getAtomName((xResourceReq *) &requestBuff);
           break;

      case X_QueryFont :
           queryFont((xResourceReq *) &requestBuff);
           break;

      case X_OpenFont :
           openFont((xOpenFontReq *) &requestBuff);
           break;

      case X_ListFonts :
           listFonts((xListFontsReq *) &requestBuff);
           break;

      case X_GetFontPath :
           getFontPath((xResourceReq *) &requestBuff);
           break;

      case X_ListFontsWithInfo :
           listFontsWithInfo((xListFontsReq *) &requestBuff);
           break;

      case X_ChangeProperty :
           changeProperty((xChangePropertyReq *) &requestBuff);
           break;

      case X_MapWindow :
           mapWindow((xResourceReq *) &requestBuff);
           break;

      case X_MapSubwindows :
           mapSubwindows((xResourceReq *) &requestBuff);
           break;

      case X_UnmapWindow :
           unMapWindow((xResourceReq *) &requestBuff);
           break;

      case X_ChangeWindowAttributes :
           changeWindowAttributes((xChangeWindowAttributesReq *) &requestBuff);
           break;

      case X_GetWindowAttributes :
           getWindowAttributes((xResourceReq *) &requestBuff);
           break;

      case X_GetGeometry :
           getGeometry((xResourceReq *) &requestBuff);
           break;

      case X_QueryTree :
           queryTree((xResourceReq *) &requestBuff);
           break;

      case X_GetProperty :
           getProperty((xGetPropertyReq *) &requestBuff);
           break;

      case X_QueryExtension :
           queryExtension((xQueryExtensionReq *) &requestBuff);
           break;

      case X_GetInputFocus :
	   getInputFocus();
	   break;

      case X_AllocColor :			// Deal with colour locally pass to gc 24bit colour
	   allocColor((xAllocColorReq *) &requestBuff);
	   break;
	  	   
      case X_AllocNamedColor :			// Deal with colour locally pass to gc 24bit colour
	   allocNamedColor((xAllocNamedColorReq *) &requestBuff);
	   break;
	 	  
//      case X_QueryColors :
//           queryColors((xQueryColorsReq *) &requestBuff);
//           break;

      case X_LookupColor :			// Deal with colour locally pass to gc 24bit colour
          lookupColor((xLookupColorReq *) &requestBuff);
	  break;	

      case X_GetSelectionOwner :
           getSelectionOwner((xResourceReq *) &requestBuff);
           break;

      case X_GrabKeyboard :
           grabKeyboard((xGrabKeyboardReq *) &requestBuff);
           break;

      case X_GetKeyboardControl :
           getKeyboardControl((xResourceReq *) &requestBuff);
           break;

      case X_GetKeyboardMapping :
           getKeyboardMapping((xGetKeyboardMappingReq *) &requestBuff);
           break;

      case X_GetModifierMapping :
           getModifierMapping((xResourceReq *) &requestBuff);
           break;

      case X_GetPointerControl :
           getPointerControl((xResourceReq *) &requestBuff);
           break;

      case X_GetScreenSaver :
           getScreenSaver((xResourceReq *) &requestBuff);
           break;

      case X_QueryBestSize :
           queryBestSize((xQueryBestSizeReq *) &requestBuff);
           break;

      //************************************************//
      //  Deal with and need to forward                 //
      //************************************************//
      case X_CreateWindow :
           createWindow((xCreateWindowReq *) &requestBuff);
           break;

      case X_ConfigureWindow :
           configWindow((xConfigureWindowReq *) &requestBuff);
           break;

      case X_DestroyWindow :
           destroyWindow((xResourceReq *) &requestBuff);
           break;

      case X_ClearArea :
           std::cerr << "Clear Area" << std::endl;
           clearArea((xClearAreaReq *) &requestBuff);
           break;
           
      case X_CreatePixmap :
           createPixmap(( xCreatePixmapReq *) &requestBuff);
           break;

      case X_FreePixmap :
	   freePixmap(&requestBuff.req);
	   break;
      //************************************************//
      //  forward - Reply expected                      //
      //************************************************//
      case X_QueryColors :
#ifdef DEBUG
           std::cerr << "Forward - Reply expected " << std::endl;
           std::cerr << "Send to java : " << len << std::endl;
#endif
           Xwindow::javasock -> write((char *) &requestBuff,len);
           break;

      //************************************************//
      //  forward with Window changed to javaWindow Id  //
      //************************************************//
      case X_PolyPoint :
      case X_PolyLine :
      case X_PolySegment :
      case X_PolyRectangle :
      case X_PolyArc :
      case X_FillPoly :
      case X_PolyFillRectangle :
      case X_PolyFillArc :
      case X_PolyText8 :
      case X_PutImage :
      case X_SetClipRectangles :
           std::cerr << "Forward with WinID " << requestBuff.req.id << std::endl;
           // requestBuff.req.id = checkJavaWid(requestBuff.req.id);
           requestBuff.req.id = rootWin -> JavaWid(requestBuff.req.id);
           if ( requestBuff.req.id > 0 ) 
              {
              std::cerr << " --> Java Id " << requestBuff.req.id << std::endl;
              std::cerr << "Send to java : " << len << std::endl;
              Xwindow::javasock -> write((char *) &requestBuff,len);
	      }
           else
              std::cerr << "Window Not Found" << std::endl;
           break;
      //************************************************//
      //  forward with changed source and dest window   //
      //************************************************//
      case X_CopyPlane :
      case X_CopyArea :
           requestBuff.req.id = rootWin -> JavaWid(requestBuff.req.id);
           requestBuff.data[0] = rootWin -> JavaWid(requestBuff.data[0]);
           if ( requestBuff.req.id > 0 && requestBuff.data[0] > 0 ) 
              {
              std::cerr << "--> Java Id " << requestBuff.req.id << std::endl;
              // requestBuff.data[0] = checkJavaWid(requestBuff.data[0]);
              std::cerr << "Send to java : " << len << std::endl;
              Xwindow::javasock -> write((char *) &requestBuff,len);
	      }
           else
              std::cerr << "Window Not Found" << std::endl;
           break;
      //************************************************//
      //  Ignore for Now                                //
      //************************************************//

      //case X_SetClipRectangles :
	   			      
      case X_FreeGC :
      case X_CreateGlyphCursor :
      case X_ChangeGC :
      case    129 :     // Shape extension
           break;

      //************************************************//
      //  forward no reply                              //
      //************************************************//
      case X_CreateGC :
	   std::cerr << "Create GC" << std::endl;
	   Xwindow::javasock -> write((char *) &requestBuff,len);
	   break;
			
      default :
#ifdef DEBUG
           std::cerr << "Forward No Reply" << std::endl;
           std::cerr << "Send to java : " << len << std::endl;
#endif
           Xwindow::javasock -> write((char *) &requestBuff,len);
           break;
      }
   return(1);           // DO one request per select for now
   }
return(1);
}

void closeDownClient(void)
{
    std::cerr << "Close Down X Client" << std::endl;
    // Java Client has gone away
    // Close session with Xserver
    XCloseDisplay(display);
    // Close java socket
    std::cerr << "Close Java Socket" << std::endl;
    Xwindow::javasock -> close();
    // Close X11 socket
    std::cerr << "Close X Socket" << std::endl;
    Xwindow::X11sock -> close();
    // End task
    std::cerr << "Kill Client" << std::endl;
    kill(setupPid,SIGKILL);
    std::cerr << "exit" << std::endl;
    exit(0);
}
//////////////////////////////////
// Process Replies	        //
//------------------------------//
// Returns 	                //
//     n  - Number processed	//
//     0  - End of File	        //
//    -ve - Error		//
//////////////////////////////////
int processReplies(void)
{
int ret,len;
short x,y;
Xwindow *winPtr;

struct
  {
  union
    {
    xGenericReply reply;
    xEvent        event;
    } u;
  char data[1024];
  } buff;

std::cerr << "Process Replies" << std::endl;

std::memset(&buff,0,32);
if (( len =  Xwindow::javasock -> readBlock((char *) &buff,32)) != 32 )
   {
   if ( len == 0 )
      {
      std::cerr << "End of Java Stream" << std::endl;
      closeDownClient();
      }
   else
      {
      std::cerr << "Error Reading basic Reply from java" << std::endl;
      return(-2);
      }
   }
len = buff.u.reply.length << 2;
if ( len > 0 )
   {
   if (( ret = Xwindow::javasock -> readBlock(buff.data,len)) != len )
      {
      if ( ret == 0 )
	     {
	     std::cerr << "End of Java Stream" << std::endl;
	     closeDownClient();
         }
      else
         {
         std::cerr << "Error Reading additional data from java" << std::endl;
         return(-2);
         }
      }
   }
len += 32;

// Update sequence Number as not all passed to java
buff.u.reply.sequenceNumber = sequenceNum;
switch ( buff.u.reply.type )
   {
   case 1 : // Reply
   		std::cerr << "Reply from Java Client" << std::endl;
        break;

   case 2 : // KeyPress
   case 3 : // KeyRelease
   case 4 : // Button Press
   case 5 : // Button Release
        std::cerr << "Event : " << buff.u.reply.type << " Received" << std::endl;
        winPtr = rootWin -> AddressJavaWin(buff.u.event.u.keyButtonPointer.event);
//        winPtr -> rootCoord(buff.u.event.u.keyButtonPointer.eventX,
//            buff.u.event.u.keyButtonPointer.eventY,
//            &buff.u.event.u.keyButtonPointer.rootX,
//                    &buff.u.event.u.keyButtonPointer.rootY);
        winPtr -> rootCoord(buff.u.event.u.keyButtonPointer.eventX,
            buff.u.event.u.keyButtonPointer.eventY,
              &x,&y);
        std::cerr << "Java root coords " << buff.u.event.u.keyButtonPointer.rootX
             << " : " << buff.u.event.u.keyButtonPointer.rootY << std::endl;

        std::cerr << "JavaServ Calc coords " << x << " : " << y << std::endl;
        buff.u.event.u.keyButtonPointer.root = rootWin -> X11windowId(0);
        buff.u.event.u.keyButtonPointer.event = rootWin -> X11windowId(buff.u.event.u.keyButtonPointer.event);
        len = 32;
        break;

   case 65 : // This is really a Reply (1) with Window needing translation at offset 4
        buff.u.reply.type = 1;
        buff.u.reply.data00 = rootWin -> X11windowId(buff.u.reply.data00);
        break;

   default :
        std::cerr << "Unknown Event from Java : " << buff.u.reply.type << std::endl;
        break;
   }
std::cerr << "Send to X client : " << len << std::endl;
Xwindow::X11sock -> write((char *) &buff,len);
return(1);
}

int readCommand(char *command)
{
	int hl,len,ret;
struct
  {
//  int len;	
  xReq req;
  int data[1024];
  } requestBuff;
// MUST DO !!!!! Add function to read in Request as this code is repeated
// in process  
std::cerr << "Read Command" << std::endl;
// Read in Header
if (( hl = Xwindow::javasock -> readBlock((char *) &requestBuff.req,4)) != 4)
   {
   if ( hl == 0 )
      {
      std::cerr << "End of Stream" << std::endl;
      return(0);
      }
   else
       {
       std::cerr << "Error reading Header : " << hl << " Bytes Read " << std::endl;
       return(-2);
       }
   } 
// Get remaining length
// len = ( requestBuff.req.length - 1) << 2;
// X sending in 4 byte chuncks
len = requestBuff.req.length;
std::cerr << "Header Read - type : " << (int) requestBuff.req.reqType << " Length " << len << std::endl;
if ( len > 1024 )
   {
   std::cerr << "X11 Request data > 1024 bytes" << std::endl;
   return(-2);
   }
 std::cerr << "Read : " << len << " bytes" << std::endl;  
if (( ret = Xwindow::javasock -> readBlock((char *) &requestBuff.data,len)) != len )
   {
   if ( ret == 0 )
      {
      std::cerr << "End of Stream" << std::endl;
      return(0);
	  }
    else
	   {
       std::cerr << "Error reading Request data" << std::endl;
       return(-2);
	   }
    }
std::cerr << "Copy String : " << std::endl;
dumpHex((char*) requestBuff.data,len);
::strncpy(command,(char *) requestBuff.data,len);
std::cerr << "Command Read : " << command << std::endl;
}

int process(void)
{
SockSet *sockset = new SockSet();
int     ret,v;

sequenceNum  = 0;
fontEntries  = 0;
do
   {
   sockset -> init();
   sockset -> addReadMask(Xwindow::X11sock);
   sockset -> addReadMask(Xwindow::javasock);
   std::cerr << "Select on inputs" << std::endl;
   v = sockset -> select(TIMEOUT);
   if ( sockset -> isset(Xwindow::X11sock))
      if (( ret = processRequests()) <= 0 )
         return(ret);
   if ( sockset -> isset(Xwindow::javasock))
      if (( ret =  processReplies()) <= 0 )
         return(ret);
   } while ( v > 0 );
std::cerr << "Timed Out" << std::endl;
return(-3);
}

void sigHandler(int s)
{
std::cerr << "Signal Handler" << std::endl;
if ( Xwindow::javasock != NULL )
   {
   std::cerr << "Closing java socket" << std::endl;
   Xwindow::javasock -> close();
   }
if ( Xwindow::X11sock != NULL  )
   {
   std::cerr << "Closing X11 socket" << std::endl;
   Xwindow::X11sock -> close();
   }
std::cerr << "Exiting Program" << std::endl;
exit(3);
}

int exportDisplay(int d)
{
static char disp[42] = "DISPLAY=";
int i,ret;

std::cerr << "exportDisplay : " << d << std::endl;
i = strlen(disp);
if (gethostname(&disp[i],sizeof(disp) - 2 - i) == -1 )
   {
   std::cerr << "GetHostname Failed errno " << errno << std::endl;
   exit(2);
   }
i = strlen(disp);
disp[i++] = ':';
disp[i++] = '0' + d;
disp[i] = 0;
std::cerr << "export " << disp << std::endl;
if ( ret = ::putenv(disp) != 0 )
   std::cerr << "putenv failed errno " << errno << std::endl;
return(ret);
}

int setupServerX(int debug,int sockNum)
{
std::cerr << "setupServerX : " << sockNum << std::endl;
if (( display = ::XOpenDisplay(displayVariable)) == NULL )
   {
   std::cerr << "Failed to open display" << std::endl;
   exit(2);
   }
std::cerr << "Opening X11 server Socket" << std::endl;
X11Serv = new tcpSocket(AF_INET,SOCK_STREAM);
X11Serv -> setReUse();
//
// Will need to vary Socket for X as xclient such as info close
// stream and open new sock with X-server and also need to
// deal with more than one client
//
std::cerr << "Binding Socket : " << 6000 + sockNum << std::endl;
if (debug) // Are we in debugging mode ?
   {
   while ( X11Serv -> bind(INADDR_ANY,6003) < 0 )
      {
      // Socket not yet free sleep and wait
      std::cerr << "Waiting for X server Socket : 6003 to free" << std::endl;
      sleep(30);
      }
   }
else
   {
   while ( X11Serv -> bind(INADDR_ANY,6000 + sockNum) < 0 )
      {
      sockNum++;
      }
   }
std::cerr << "Listening on X11 Server " << 6000 + sockNum << std::endl;
X11Serv -> listen(5);
return(0);
}

int execClient(char *command)
{
// Very Very Lazy, use Semaphore
::sleep(2);               // sleep for 2 secs
// Execute command to run X client
std::cerr << "execl : " << command << std::endl;
if ( ::execlp(command,command,NULL) < 0 )
   {
   std::cerr << "Error execing application - errno < " 
        << errno << " > " << strerror(errno) << std::endl;
   return(-1);
   }
return(0);  
}

int inetdProcess(int dispNum)
{
int errFlag;
int setupPid;

std::cerr << "Running under inetd" << std::endl;
std::cerr << "Running under inetd NOT YET UPDATED" << std::endl;
return(-1); // Abort

javaServ = new tcpSocket(0);
setupServerX((dispNum != 3),dispNum);
errFlag = 0;
std::cerr << "Forking for X11 appl" << std::endl;
if ((setupPid = ::fork()) < 0 ) // Fork and check okay
   {
   std::cerr << "Fork failed : " << errno << std::endl;
   return(-1);
   }
if ( setupPid == 0 ) // Is this the Child - Yes execl x application
   {
   execClient("xclock");
   }
else
   {
   // Need to loop on accept in case client closes stream e.g. info
   while (errFlag == 0)
      {
      std::cerr << "Accepting X11 Server" << std::endl;
      Xwindow::X11sock  = new tcpSocket(X11Serv-> accept());
      connectionSetup();
      errFlag = process();
      }
   ::kill(setupPid,SIGQUIT);
   Xwindow::X11sock -> close();
   }
return(errFlag);
}

int listenProcess(int sockNum,int dispNum)
{
int       errFlag;

char	  command[120];

std::cerr << "ListenProcess Socket Number : " << sockNum << " Display Number : " << dispNum << std::endl;
std::cerr << "Opening Java server Socket" << std::endl;
javaServ = new tcpSocket(AF_INET,SOCK_STREAM);
javaServ -> setReUse();
std::cerr << "Binding Socket" << std::endl;
while ( javaServ -> bind(INADDR_ANY,PORT) < 0 )
   {
   std::cerr << "Waiting on Server Socket port : "<< PORT << std::endl;
   sleep(5);
   }
std::cerr << "Listening on Java Server" << std::endl;
javaServ -> listen(5);
errFlag = 0;
while ( errFlag == 0 )
   {
   std::cerr << "Accepting java Server" << std::endl;
   Xwindow::javasock  = new tcpSocket(javaServ-> accept());
   std::cerr << "Reading required X Client Application" << std::endl;
   std::memset(command,0,120);	
   readCommand(command);
   std::cerr << "Command Read : " << command << std::endl;
   std::cerr << "Main Forking" << std::endl;
   if ((setupPid = ::fork()) < 0 ) // Fork and check okay
      {
      std::cerr << "Main Fork failed : " << errno << std::endl;
      return(-1);
      }
   if ( setupPid == 0 ) // Is this the Child - Read Application
      {
      // Close main java socket
      javaServ -> close();
      std::cerr << "Forking for X Client" << std::endl;
      if ((setupPid = ::fork()) < 0 ) // Fork and check okay
      	 {
         std::cerr << "Fork failed for X Client : " << errno << std::endl;
         return(-1);
         }
      if ( setupPid == 0 ) // Is this the Child - Exec X Client
         {
         // Close java socket
         Xwindow::javasock -> close();
      	 // Xwindow::javasock -> close();
      	 // Export appropriate DISPLAY variable
	 exportDisplay(dispNum);
         execClient(command);
         }
      else // Accept data from X11 Client and process
         {	
         setupServerX(True,sockNum);
         while (errFlag == 0)
             {
             std::cerr << "Accepting X11 Server" << std::endl;
             Xwindow::X11sock  = new tcpSocket(X11Serv-> accept());
             std::cerr << "Close Main X11 socket" << std::endl;
             X11Serv -> close();
             connectionSetup();
             // Process requests from X11 Client
             errFlag = process();
             }
         } 
      }
   else
      {
      // This is root Parent Task go back to accepting new applets
      std::cerr << "Root Process Close socket and back to Accepting" << std::endl;
      Xwindow::javasock -> close();
      }
   }
return(0);
}

int main(int argc,char **argv)
{
int       i,dispNum,sockNum;
Font      font;
int       setupPid;
int       errFlag;

// setbuf(stdout,NULL);
setbuf(stdout,0);
::signal(SIGQUIT,sigHandler);
::signal(SIGTERM,sigHandler);
::signal(SIGINT,sigHandler);
::signal(SIGUSR1,sigHandler);

std::cerr << "JavaXServ version 1.0 alpha Copyright: Keith Sloan 1997,2008" << std::endl;
dispNum = sockNum = 3;
if ( argc > 1 )
   {
   sockNum = atoi(argv[1]);
   }
if ( argc > 2 )
   {
   dispNum = atoi(argv[2]);
   }
std::cerr << "Socket Number : " << sockNum << " Display Number : " << dispNum << std::endl;   
if ((displayVariable = ::getenv("DISPLAY")) == 0 )
   {
   std::cerr << "DISPLAY variable Not set" << std::endl;
   exit(2);
   }
   std::cerr << "About to open display" << std::endl;
if (( display = ::XOpenDisplay(displayVariable)) == NULL )
   {
   std::cerr << "Failed to open display" << std::endl;
   exit(2);
   }
// Create root window ( same parameters as connection setup
rootWin = new Xwindow(-1,XDefaultRootWindow(display),NULL,0,0,0,600,800,8,1);
if ( ::strstr(argv[0],"ijavaServ" ) == 0 )
   {
   listenProcess(sockNum,dispNum);
   }
else
   {
   inetdProcess(dispNum);
   }
return(0);
}

