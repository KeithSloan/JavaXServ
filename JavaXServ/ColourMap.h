#ifndef DEFColourMap
#define DEFColourMap

#define COL_MAP_ENTRIES 256

class ColourMap; 
    static ColourMap *ColMapfirst;
    static ColourMap *ColMaplast;
    static int lastId;

class ColourMap
    {
    private:
	int     id;
	int	index;
	int     downloaded;
	int     colours[COL_MAP_ENTRIES];
	ColourMap  *ColMapnext;

    public:
	ColourMap();
	ColourMap(int i);
	~ColourMap();
	ColourMap *Address(int i);
	int getId();
	int getDownLoaded();	
	int AllocColour(int c);
        void sendColourMap();
    };
#endif 
	
