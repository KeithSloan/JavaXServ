#include <iostream>
#include "ColourMap.h"
#include "Xwindow.h"

extern "C"              // C includes
{
#define NEED_EVENTS
#define NEED_REPLIES
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
}
    
ColourMap::ColourMap(int i)
    {
    std::cerr << "Colour Map Initial Constructor" << std::endl;
    id = i;
    ColMapfirst = this;
    ColMaplast	= this;
    ColMapnext	= NULL;
    index 	= 2;
    colours[0]  = 0x00FFFFFF;
    colours[1]  = 0;
    lastId 	= ++i;
    }

ColourMap::ColourMap()
    {
    ColMaplast -> ColMapnext = this;
    id 		= lastId;
    ColMapfirst = this;
    ColMaplast 	= this;
    ColMapnext 	= NULL;
    index	= 0;
    lastId++;
    }

ColourMap::~ColourMap(void)
    {
    }

int ColourMap::getId()
    {
    return(id);
    }

ColourMap *ColourMap::Address(int i)
    {
    ColourMap *ptr = ColMapfirst;
    std::cerr << "Address : " << ptr << std::endl;
    while ( ptr != NULL )
        {
	if (ptr->id == i ) return(ptr);
        ptr = ptr -> ColMapnext; 
	} 
    return(NULL);   
    }

int ColourMap::AllocColour(int c)
    {
    int r = index;
    if (index < 256 )
       {
       colours[index++] = c;
       return(r);
       }
    else
       {
       std::cerr << "Colour Map Full" << std::endl;
       }
    }

int ColourMap::getDownLoaded()
   {
   return downloaded;
   }
 
void ColourMap::sendColourMap()
{
struct
    {
    CARD8 reqType;
    BYTE pad;
    CARD16 length B16;
    Colormap map B32;
    CARD16 entries B16;
    CARD16 filler B16;
    } xSendMapReq;
std::cerr << "Send Colour Map : " << id << " Entries : " << index << std::endl;
xSendMapReq.reqType = 201;
xSendMapReq.length = 3 + index;
xSendMapReq.map = id;
xSendMapReq.entries = index;
Xwindow::javasock -> write((char *) &xSendMapReq,sizeof(xSendMapReq));
Xwindow::javasock -> write((char *) colours,index << 2);
downloaded = 1;
}
	
