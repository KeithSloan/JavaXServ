#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

Display *display;
int screen;
Window window;
GC gc;
int chartreuse;

void init_x() {
	/* get the colors black and white (see section for details) */
	unsigned long black,white;

	/* use the information from the environment variable DISPLAY 
	   to create the X connection:
	*/	
	display=XOpenDisplay((char *)0);
   	screen=DefaultScreen(display);
	black=BlackPixel(display,screen),	/* get color black */
	white=WhitePixel(display,screen);       /* get color white */

	/* once the display is initialized, create the window.
	   This window will be have be 200 pixels across and 300 down.
	   It will have the foreground white and background black
	*/
   	window=XCreateSimpleWindow(display,DefaultRootWindow(display),0,0,	
		200, 300, 5, black, white);

	/* here is where some properties of the window can be set.
	   The third and fourth items indicate the name which appears
	   at the top of the window and the name of the minimized window
	   respectively.
	*/
	XSetStandardProperties(display,window,"My Window","HI!",None,NULL,0,NULL);

	/* this routine determines which types of input are allowed in
	   the input.  see the appropriate section for details...
	*/
	XSelectInput(display, window, ExposureMask|ButtonPressMask|KeyPressMask);

	/* create the Graphics Context */
        gc=XCreateGC(display, window, 0,0);        

	/* here is another routine to set the foreground and background
	   colors _currently_ in use in the window.
	*/
	XSetBackground(display,gc,white);
	XSetForeground(display,gc,black);

	/* clear the window and bring it on top of the other windows */
	XClearWindow(display, window);
	XMapRaised(display, window);
};

void close_x() {
/* it is good programming practice to return system resources to the 
   system...
*/
	XFreeGC(display, gc);
	XDestroyWindow(display,window);
	XCloseDisplay(display);	
	exit(1);				
};

void redraw()
{
XClearWindow(display, window);
XSetForeground(display,gc,chartreuse);
//XDrawArc(display,window,gc,45,1, 17,17,0,23040);
XDrawRectangle(display, window, gc, 10, 10, 100, 100);
};


void main()
{
char text[256];
XEvent event;		/* the XEvent declaration !!! */
KeySym key;
XColor tmp;

	

init_x();

XParseColor(display, DefaultColormap(display,screen), "chartreuse", &tmp);
XAllocColor(display,DefaultColormap(display,screen),&tmp);
chartreuse=tmp.pixel;
XSetForeground(display,gc,chartreuse);
//XDrawArc(display,window,gc,45,1, 17,17,0,23040);
XDrawRectangle(display, window, gc, 10, 10, 30, 30);
 
/* look for events forever... */
while(1) 
   {		
   /* get the next event and stuff it into our event variable.
      Note:  only events we set the mask for are detected!
		*/
   XNextEvent(display, &event);
	
   if (event.type==Expose && event.xexpose.count==0)
      {
      /* the window was exposed redraw it! */
      redraw();
      }
   if (event.type==KeyPress && XLookupString(&event.xkey,text,255,&key,0)==1)
      {
      /* use the XLookupString routine to convert the invent
      KeyPress data into regular text.  Weird but necessary...
      */
      if (text[0]=='q')
        {
        close_x();
        }
      }
    }
}
