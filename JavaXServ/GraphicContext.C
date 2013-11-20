#include <iostream>
#include "GraphicContext.h"
    
GraphicContext::GraphicContext()
    {
    id = 0;
    colourMap = 0;
    GCfirst = this;
    GClast = this;
    GCnext = NULL;
    }

GraphicContext::GraphicContext(int i,int cm)
    {
    GClast -> GCnext = this;
    GClast = this;
    GCnext = NULL;
    id = i;
    colourMap = cm;
    }
	
GraphicContext::~GraphicContext(void)
    {
    }

GraphicContext *GraphicContext::Address(int i)
    {
    GraphicContext *ptr = GCfirst -> GCnext;
    do  {
        ptr = ptr -> GCnext; // We can ignore first GC
	if (ptr -> id == i )
	   return(ptr);
	} while ( ptr -> GCnext != NULL );
    std::cerr << "Graphics Context : " << i << "Not found" << std::endl;   
    }

int GraphicContext::getColourMap()
    {
    return(colourMap);
    }
 
	
