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
import java.awt.Color;

public class ColourMapSet
   {
   // Vector GCSet = new Vector();
   private Vector ColourMapSet;
   // Use public for speed
   
   public ColourMapSet()
	{
	super();
	Trail("Colour Map Set");
	ColourMapSet = new Vector();
	}

   private static String hex(int n)
      {
      // call toUpperCase() if that's required
      return String.format("0x%8s", Integer.toHexString(n)).replace(' ', '0');
      }

   private void Trail(String s)
      {
      System.out.println(s);
      }
   
   public void add(ColourMap cm)
	{
	Trail("Colour Map Set - add");
	ColourMapSet.addElement(cm);
	}
   
   public ColourMap address(int ident)
	{
	ColourMap cm;
	Enumeration e = ColourMapSet.elements();
	   do 
	   {
	   cm = (ColourMap) e.nextElement();
	   } while (e.hasMoreElements() && cm.getIdent() != ident);
	if ( cm.getIdent() != ident )
	   {
	   Trail("Colour Map Not found");
	   return(null);
	   }
	else
	   {
	   return cm;
	   }
       }
         
   public void delete(int ident)
	{
	ColourMap cm = address(ident);
	ColourMapSet.remove(cm);
	}
   }
