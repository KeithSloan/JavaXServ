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

import java.awt.*;
import java.awt.event.*;

public class BasePanel extends Panel 
   {
   private static final long serialVersionUID = 1L;
   MapLayout ml;
   XkeyBoard keyboard;
   int	     index;	
   int       width;
   int       height;

   
public BasePanel(int i,XkeyBoard k,int w,int h,Color bgc)
     {
     super();
     width  = w;
     height = h; 
     index  = i;
     keyboard = k;
     super.setBackground(bgc);  
     ml     = new MapLayout(width,height);
     super.setLayout(ml);
     }

private void Trail(String s)
   {
   System.out.println(s);
   }
   
public void paint(Graphics g)
   {
   }
 
public void registerChild(Xwindow w)
    {
    Trail("BasePanel Register Child");
 //     add("1",(Component) w);
    add((Component) w.window,"1");
    ml.setConstraint(w.window,w.xPos-w.border,w.yPos-w.border,w.width+2*w.border,w.height+2*w.border);
    layout();
    }

public void resetChild(Xwindow c)
    {
    Trail("BasePanel Reset Child");
    c.window.resize(c.width,c.height);
    ml.setConstraint(c.window,c.xPos-c.border,c.yPos-c.border,c.width+2*c.border,c.height+2*c.border);
    layout();
    }

public void removeChild(Xwindow w)
    {
    Trail("BasePanel Remove Child");
    ml.removeLayoutComponent((Component) w.window);
    }

//private void sendKeyEvent(Event e,int code,int detail,int state)
//    {
//    Rectangle r;
//
//    r = bounds();    
//    sock.replyEvent(code);
//    sock.addByte(detail);
//    sock.addCard16(0);		// sequence number updated by javaServ
//    // sock.addCard32((int) e.when);	// time
//    sock.addCard32(0);		// time  0 => Current
//    sock.addCard32(0);		// root Window
//    sock.addCard32(index);	// updated by javaServ
//    sock.addCard32(0);		// Child
//    sock.addCard16(r.x + e.x);
//    sock.addCard16(r.y + e.y);
//    sock.addCard16(e.x);
//    sock.addCard16(e.y);
//    sock.addCard16(state);	// State
//    sock.addByte(1);		// Same Screen = true
//    sock.addByte(0);		// unused 
//    sock.flushEvent();
//    }

//private int keyMask(Event e)
//    {
//    int r = 0;
//    if ( e.shiftDown())   r += 1;
//    if ( e.controlDown()) r += 4; 
//    return(r);
//    }

   
//   void processMouseEvent(MouseEvent e)
//   {
//	   Trail("Mouse Event from Jwindow : "+index+" Hooray !!!"); 
//   }
   
//   void 	processKeyEvent(KeyEvent e) 
//   {
//	   Trail("Key Event from Jwindow : "+index+" Hooray !!!"); 
//   }
   
public boolean handleEvent(Event e)
    {
    int k;
    Rectangle r;

    r = bounds();
    // Trail("Handle Event from BasePanel : "+index+" Hooray !!!");
    switch ( e.id )
       {
       case Event.KEY_PRESS :
            k = keyboard.KeySymToCode(e.key);
	    Trail("Key : "+e.key+" KeySym : "+k);
	    keyboard.sendKeyEvent(index,r,e,2,k,keyboard.keyMask(e));
            break;

       case Event.KEY_RELEASE:
	    keyboard.sendKeyEvent(index,r,e,3,keyboard.KeySymToCode(e.key),keyboard.keyMask(e));
	    break;

       case Event.MOUSE_DOWN:
	    keyboard.sendKeyEvent(index,r,e,4,1,1);
	    break;

       case Event.MOUSE_UP:
            keyboard.sendKeyEvent(index,r,e,5,1,0x100);
            break;
        	  
       case Event.LOST_FOCUS:
	    System.out.println("Lost Focus");
	    break;
       }
    return(true);
    }


   // Used for debugging Only ( root window has no image )
public void drawImage(Image img,int x,int y,int w,int h)
   {
   Graphics g = getGraphics();

   g.drawImage(img,x,y,w,h,this);
   }

}

