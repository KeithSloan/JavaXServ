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

import java.util.*;

public class GraphicsContextSet
   {
   // Vector GCSet = new Vector();
   private Vector GCSet;
   private int    currentGC;
   // Use public for speed
   public  int    foreGround;
   public  int	  backGround;
   public  int	  lineWidth;
   public  int	  lineStyle;
   public  int	  font;
  
   public GraphicsContextSet()
	{
	super();
	Trail("Graphics Context Set");
	GCSet = new Vector();
	currentGC = 0;
	}

   private void Trail(String s)
      {
      System.out.println(s);
      }

   public void setGC(int id)
	{
        if (id == currentGC)
	   { 
           return; // return No change
	   }
	else
	   {
           GraphicsContext gc = address(id);
	   foreGround = gc.foreGround;
	   backGround = gc.backGround;
	   lineWidth  = gc.lineWidth;
           lineStyle  = gc.lineStyle;
           font	      = gc.font;
	   currentGC  = id;
	   }
	}
   
   public void add(int id, int bm, int fore, int back, int lw, int ls, int fnt)
	{
	Trail("GraphicsContextSet - add");
	GraphicsContext gc = new GraphicsContext();
	gc.SetValues(id,bm,fore,back,lw,ls,fnt);
	GCSet.addElement(gc);
	}
   
   public GraphicsContext address(int ident)
	{
	GraphicsContext gc;
	Enumeration e = GCSet.elements();
	   do 
	   {
	   gc = (GraphicsContext) e.nextElement();
	   } while (e.hasMoreElements() && gc.getIdent() != ident);
	if ( gc.getIdent() != ident )
	   {
	   Trail("GC Not found");
	   return(null);
	   }
	else
	   {
	   return gc;
	   }
       }
         
   public void delete(int ident)
	{
	GraphicsContext gc = address(ident);
	GCSet.remove(gc);
	}
   }
