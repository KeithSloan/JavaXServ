#ifndef DEFGraphicContext
#define DEFGraphicContext

#include "ColourMap.h"

class GraphicContext; 
        static GraphicContext *GCfirst;
        static GraphicContext *GClast;

class GraphicContext
    {
    private:
	int id;
	int colourMap;
	GraphicContext  *GCnext;

    public:
	GraphicContext();
	GraphicContext(int i,int cm);
	~GraphicContext();
	GraphicContext *Address(int id);
        int getColourMap();
    };
#endif 
	
