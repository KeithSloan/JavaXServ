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

public class GraphicsContext
   {
   GraphicsContext	next;
   GraphicsContext	previous;
   int			cid;
   int			bitmask;
   // Use public for speed
   public int		foreGround;
   public int		backGround;
   public int		lineWidth;
   public int		lineStyle;
   public int		font;
   
   public GraphicsContext()
	{
	next = previous = null;
	}

   private void Trail(String s)
	{
	// System.out.println(s);
	}
  
   public void SetValues(int id,int bm, int fore, int back, int lw, int ls, int fnt )
   	{
	cid = id;
	bitmask = bm;
	foreGround = fore;
	backGround = back;
	lineWidth = lw;
	lineStyle = ls;
	font = fnt;
   	}
   
   public GraphicsContext getNext()
	{
	return(next); 
	}
   
   public GraphicsContext getPrevious()
	{
	return(previous);
	}
   
   public void setNext(GraphicsContext gc)
	{
	next = gc;
      	}
      
   public void setPrevious(GraphicsContext gc)
	{
	previous = gc;
	}
   
   public int  getIdent()
	{
	return(cid);
        }
   }

   
