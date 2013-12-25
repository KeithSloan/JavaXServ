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

public class ColourMap
   {
   ColourMap	next;
   ColourMap	previous;
   int		mapId;
   int		entries;
   // Have to store RGB values as Index Colour depends on putimage
   byte		Reds[];
   byte		Greens[];
   byte		Blues[];

   public ColourMap()
        {
        }
   
   public ColourMap(int id,int e)
	{
        mapId = id;
	entries = e;
	Reds   = new byte[entries];
   	Greens = new byte[entries];
   	Blues  = new byte[entries];
	next = previous = null;
	}

   private void Trail(String s)
	{
	System.out.println(s);
	}

   public static String hex(int n)
	{
	// call toUpperCase() if that's required
	return String.format("0x%2s", Integer.toHexString(n & 0xFF)).replace(' ', '0');
	}
   
   public ColourMap getNext()
	{
	return(next); 
	}
   
   public ColourMap getPrevious()
	{
	return(previous);
	}
   
   public void setNext(ColourMap cm)
	{
	next = cm;
      	}
      
   public void setPrevious(ColourMap cm)
	{
	previous = cm;
	}
   
   public int  getIdent()
	{
	return(mapId);
        }

   public void readColourMap(Xsocket sock,int entries)
	{
	int i;

	for ( i=0; i<entries; i++ )
	    {
	    Blues[i]  = (byte) sock.readByte(false);
	    Greens[i] = (byte) sock.readByte(false);
	    Reds[i]   = (byte) sock.readByte(false);	
	    sock.readByte(false);
	    Trail(" RGB - Index : "+hex(i)+" Red : "+hex(Reds[i])+" Green : "+hex(Greens[i])+" Blue : "+hex(Blues[i]));
	    }           
	}

   public IndexColorModel returnIndexColModel(int bits)
	{
        Trail("Return IndexColorModel : bits "+bits+" entries "+entries);
	return(new IndexColorModel(bits,entries,Reds,Greens,Blues));
	} 
   }

   
