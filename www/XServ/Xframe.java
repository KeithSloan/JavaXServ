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
import java.awt.image.*;

public class Xframe extends Frame 
   {
   private static final long serialVersionUID = 1L;
   MapLayout ml;
   XkeyBoard keyboard;
   int       index;

   public Xframe(int i,XkeyBoard k,int w,int h,int bgc)
      {
      super();
      index = i;
      keyboard = k;
      super.setBackground(new Color(bgc));
      addNotify();
      resize(w+20,h+60);
      ml = new MapLayout(w+20,h+60);
      setLayout(ml);
      show();
      } 

   private void Trail(String s)
   {
   System.out.println(s);
   }
   
   public Xwindow create(int i,XkeyBoard k, int p,int x,int y,int w,int h)
      {
      Xwindow win = new Xwindow(i,k,p,x,y,w,h);
      return(win);
      }

   public void registerChild(Xwindow w)
      {
      Trail("Frame Register Child");
      // add("1",(Component) w); 
      add((Component) w.window,"1");
      ml.setConstraint(w.window,w.xPos+10,w.yPos+50,w.width+2*w.border,w.height+2*w.border);
      layout();
      }

   public void resetChild(Xwindow w)
      {
      Trail("Frame Reset Child : border :"+w.border);
      w.window.resize(w.width,w.height);	// This is without borders
      ml.setConstraint(w.window,w.xPos+10,w.yPos+50,w.width+2*w.border,w.height+2*w.border);
      layout();
      }
   public boolean handleEvent(Event e)
    {
    int k;
    Rectangle r;

    r = bounds();
//    Trail("Frame Handle Event : "+e.id);
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

       case Event.WINDOW_DESTROY:
            System.out.println("Close Window");
	    dispose();
            break;
       }
    return(true);
    }
}
