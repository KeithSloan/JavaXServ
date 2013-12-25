// compile with
// gcc -lX11 -lXpm t.c
	 
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include "foo.xpm"
// yes, really, just throw the file in
	 
#define W_WIDTH 100
#define W_HEIGHT 100
 
int showImage(Display *dis, Window win, char *xpm[]) {
XImage *img;
if (XpmCreateImageFromData (dis, xpm, &img, NULL, NULL)) {
   fprintf(stderr, "Can't read image.\n");
   return 0;
} else {
   int x = (W_WIDTH - img->width)/2;
   int y = (W_HEIGHT - img->height)/2;
   GC gc = XCreateGC(dis, win, 0, NULL);
   XPutImage(dis, win, gc, img, 0, 0, x, y, img->width, img->height );
   XFreeGC(dis, gc);
   return 1;
   }
}
	 
int main() {
Display *dis = XOpenDisplay(NULL);
int blackColor = BlackPixel(dis, DefaultScreen(dis));
 
Window win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, W_WIDTH, W_HEIGHT, 0, blackColor, blackColor);
XSelectInput(dis, win, StructureNotifyMask);
XMapWindow(dis, win);
 
for(;;) {
    XEvent e;
    XNextEvent(dis, &e);
    if (e.type == MapNotify) { break; }
    }
 
if (showImage(dis, win, foo_xpm)) {
   XFlush(dis);
   getchar();
   }
return 0;
}


