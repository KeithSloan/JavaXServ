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

public class Jwindow extends Panel 
   {
   private static final long serialVersionUID = 1L;
   MapLayout ml;
   Xsocket   sock;
   Image     image;
   int	     index;	
   int       width;
   int       height;
   int       border;

public Jwindow(Xsocket s,Image img,int i,int w,int h)
     {
     super();
     width  = w;
     height = h; 
     index  = i;
     sock   = s;
     image  = img;
     ml     = new MapLayout(width,height);
     super.setLayout(ml);
     } 
   
public Jwindow(Xsocket s,int i,int w,int h)
     {
     super();
     width  = w;
     height = h; 
     index  = i;
     sock   = s;
     image  = null;
     ml     = new MapLayout(width,height);
     super.setLayout(ml);
     }

private void Trail(String s)
   {
   System.out.println(s);
   }
   public void paint(Graphics g)
      {
      g.setColor(Color.black);
      if ( image != null )
         {
         g.drawImage(image,0,0,this);
         }
      else
         {
         System.out.println("Pain jWindow : "+index+" which has null image");
         }
      g.drawRect(0,0,width-1,height-1);
      // g.drawString("Win "+index,5,20);
      }
 
   public void registerChild(Xwindow w)
      {
      Trail("Register Child");
 //     add("1",(Component) w);
      add((Component) w.window,"1");
      ml.setConstraint(w.window,w.xPos,w.yPos,w.width,w.height);
      layout();
      }

   public void resetChild(Xwindow c)
      {
      Trail("Reset Child");
      ml.setConstraint(c.window,c.xPos,c.yPos,c.width,c.height);
      layout();
      }

   public void removeChild(Xwindow w)
      {
      Trail("Remove Child");
      ml.removeLayoutComponent((Component) w.window);
      }

   private void sendKeyEvent(Event e,int code,int detail,int state)
      {
      Rectangle r;

      r = bounds();    
      sock.replyEvent(code);
      sock.addByte(detail);
      sock.addCard16(0);		// sequence number updated by javaServ
      // sock.addCard32((int) e.when);	// time
      sock.addCard32(0);		// time  0 => Current
      sock.addCard32(0);		// root Window
      sock.addCard32(index);		// updated by javaServ
      sock.addCard32(0);		// Child
      sock.addCard16(r.x + e.x);
      sock.addCard16(r.y + e.y);
      sock.addCard16(e.x);
      sock.addCard16(e.y);
      sock.addCard16(state);		// State
      sock.addByte(1);			// Same Screen = true
      sock.addByte(0);			// unused 
      sock.flushEvent();
      }

   private int keyMask(Event e)
      {
      int r = 0;
      if ( e.shiftDown())   r += 1;
      if ( e.controlDown()) r += 4; 
      return(r);
      }

   
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

      // Trail("Handle Event from Jwindow : "+index+" Hooray !!!");
      switch ( e.id )
         {
         case Event.KEY_PRESS :
    	      k = KeySymToCode(e.key);
	          Trail("Key : "+e.key+" KeySym : "+k);
	          sendKeyEvent(e,2,k,keyMask(e));
              break;

         case Event.KEY_RELEASE:
	          sendKeyEvent(e,3,KeySymToCode(e.key),keyMask(e));
	          break;

         case Event.MOUSE_DOWN:
	          sendKeyEvent(e,4,1,1);
	          break;

         case Event.MOUSE_UP:
        	  sendKeyEvent(e,5,1,0x100);
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

   private char KeySymToCode(int v)
      {
      char SYMtoCODE[] = {
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x00
          0,   0,   0,   0,   0,   0,  51,   0,   // 0x00
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x10
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x18
         69,  10,  11,  19,  13,  14,  15,  16,   // 0x20
         17,  18,  49,  48,  61,  20,  62,  63,   // 0x28
         19,  10,  11,  12,  13,  14,  15,  16,   // 0x30
         17,  18,  49,  48,  61,  20,  62,  63,   // 0x38
         35,  39,  58,  56,  41,  27,  42,  43,   // 0x40
         44,  32,  45,  46,  47,  60,  59,  33,   // 0x48
         34,  25,  28,  40,  29,  57,   0,  26,   // 0x50
         55,  30,  54,  36,   9,  50,  49,  53,   // 0x58
         35,  39,  58,  56,  41,  42,  43,  43,   // 0x60
         44,  32,  45,  46,  47,  60,  59,  33,   // 0x68
         34,  25,  28,  40,  29,  31,  57,  26,   // 0x70
         55,  30,  54,   0,  53,   0,  70,   0,   // 0x78
          0,   0,   0,  36,   0,   0,  36,   0,   // 0x80
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x88
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x90
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x98
          0,   0,   0,  12,   0,   0,   0,   0,   // 0xa0
          0,   0,   0,   0,  21,   0,   0,   0,   // 0xa8
         19,  18,  11,  12,   0,   0,   0,   0,   // 0xb0
          0,   0,   0,   0,   0,  14,   0,   0,   // 0xb8
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xc0
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xc8
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xd0
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xd8
          0,  52,  65,  66,   0,   0,   0,   0,   // 0xe0
          0,  69,   0,   0,   0,   0,   0,  38,   // 0xe8
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xf0
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xf8
        };

      return(SYMtoCODE[(int) v]);
      }
   }

