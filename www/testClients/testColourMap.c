#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

Display *display;
int screen;
Window windowId;

void init_x() {
	/* get the colors black and white (see section for details) */
	unsigned long black,white;

	/* use the information from the environment variable DISPLAY 
	   to create the X connection:
	*/	
	display=XOpenDisplay((char *)0);
   	screen=DefaultScreen(display);
	windowId = XDefaultRootWindow(display);
	
};

void listColourMaps()
{
Colormap *ptr;
int      i,count;

ptr = XListInstalledColormaps(display,windowId,&count);
printf("Installed Colour Maps : %d\n",count);
for ( i = 0; i < count; i++ )
    {
    printf("Colour Map : %d",(int) *ptr);
    printf(" 0x%08x\n",(int) *ptr);
    ptr++;
    }
}



Colormap initialColourMap()
{
// We need to create our own Colour map for reentry and to match Initial connection

Screen *screen;

Visual    visual;
Colormap  cm;

memset(&visual,0,sizeof(visual));
visual.visualid = 34;
//visual.class = StaticColor;
//visual.class = PseudoColor;
visual.class = DirectColor;
visual.red_mask = 0xFF0000;
visual.green_mask = 0xFF00;
visual.blue_mask = 0xFF;
visual.bits_per_rgb = 8;
visual.map_entries = 256;
printf("Create Colour Map : Window : %x\n",(int) windowId);
cm = XCreateColormap(display,windowId,&visual,AllocNone);
printf("Colour Map Created: %d\n",(int) cm);
printf("0x%08x\n",(int) cm);
printf("Install Colour Map\n");
XInstallColormap(display,cm);
return(cm);
}

void allocColour(Colormap cm,int red,int green,int blue)
{
Status status;
XColor xcolour;
xcolour.red = red;
xcolour.green = green;
xcolour.blue = blue;
xcolour.flags = DoRed | DoGreen | DoBlue;
status = XAllocColor(display,cm,&xcolour);          
printf("Status %d\n",status);
if (status == 0 )
   printf("XAllocColor failed");
printf("Pixel allocated %d %x\n",(int) xcolour.pixel,(int) xcolour.pixel);
}

void queryColours(Colormap cm,int e)
{
XColor colmap[256];
Status status;

printf("Query Colour Map : %d %x entries %d\n",(int)cm,(int)cm,e);
status = XQueryColors(display,cm,colmap,2);
if (status == 0 ) printf("XQueryColors failed\n");
}

void listVisuals()
{
int i,count;
XVisualInfo info,*ptr;

ptr = XGetVisualInfo(display,0,&info,&count);
printf("Count of Visuals : %d\n",count);
for ( i=0; i<count; i++)
    {
    printf("VisualId : %d : class %d depth : %d mapsize %d bitPerRGB %d\n",ptr->visualid,ptr->class,ptr->depth,ptr->colormap_size,ptr->bits_per_rgb);
    ptr++;
    }
}

void main()
{	
Colormap cm;
init_x();
listVisuals();
cm = initialColourMap();
listColourMaps();
//cm = XDefaultColormap(display,screen);
printf("Colour Map : %x\n",(int) cm);
allocColour(cm,255*0xFF,0,0);
allocColour(cm,0,255*0xFF,0);
allocColour(cm,0,0,255*0xFF);
queryColours(cm,2);
}
