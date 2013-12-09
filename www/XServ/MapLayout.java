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
import java.util.*;

public class MapLayout implements LayoutManager
   {
   Dimension d;
   Vector components = new Vector(10,10);
   Vector constraints = new Vector(10,10);

   public MapLayout(int w,int h) 
      {
      d = new Dimension(w,h);
      }

   private void Trail(String s)
      {
      // System.out.println(s);
      }

   public void addLayoutComponent(String name, Component comp)
      {
      Trail("Add Component");
      components.addElement((Object) comp);
      constraints.addElement((Object) null);
      }

   public void removeLayoutComponent(Component comp)
      {
      int i = components.indexOf(comp);
      if ( i != -1 )
         {
         components.removeElementAt(i);
         constraints.removeElementAt(i);
	 }
      }

   public Dimension minimumLayoutSize(Container target)
      {
      return d;
      }

   public Dimension preferredLayoutSize(Container target)
      {
      return d;
      }

   public void setConstraint(Component self,int x,int y,int w,int h)
      {
      Trail("Set Constaint : "+x+" : "+y+" : "+w+" : "+" : "+h);
      int i = components.indexOf(self);
      if ( i != -1 )
         {
         Rectangle r = new Rectangle(x,y,w,h);
         constraints.setElementAt(r,i);
         }
      }

   public void layoutContainer(Container target)
      {
      Enumeration e = components.elements();
      Rectangle r;
      Component c;
  
      Trail("Layout Container");
      int i = -1;
      while ( e.hasMoreElements())
         {
         i = i + 1;
	 r = (Rectangle)constraints.elementAt(i);
         c = (Component)e.nextElement();
         c.reshape(r.x,r.y,r.width,r.height);
         }
      }
   }
