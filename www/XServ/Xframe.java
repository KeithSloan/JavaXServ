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

   public Xframe(int w,int h)
      {
      super();
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
   
   public Xwindow create(Xsocket s,int i,int p,int x,int y,int w,int h)
      {
      Xwindow win = new Xwindow(s,i,p,x,y,w,h);
      return(win);
      }

   public void registerChild(Xwindow w)
      {
      Trail("Frame Register Child");
      // add("1",(Component) w); 
      add((Component) w.window,"1");
      ml.setConstraint(w.window,w.xPos+10,w.yPos+50,w.width,w.height);
      layout();
      }

   public void resetChild(Xwindow w)
      {
      Trail("Frame Reset Child");
      w.window.resize(w.width,w.height);
      ml.setConstraint(w.window,w.xPos+10,w.yPos+50,w.width,w.height);
      layout();
      }

   }
