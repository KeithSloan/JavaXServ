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


// Panel borders can be done with Swing, but it appears from documentation
// that this only allows one root window so implementing in awt as paenl within panel


import java.awt.*;
import java.awt.event.*;

public class BorderPanel extends Panel 
   {
   private static final long serialVersionUID = 1L;
   MapLayout ml;
   BasePanel inner;
   int	     index;	
   int       width;
   int       height;
   int       border;
   Color     borderColour;

//public BorderPanel(Xsocket s,Image img,int i,int w,int h)
//     {
//     super(s,img,i,w,h);
//     width  = w;
//     height = h; 
//     index  = i;
//     ml     = new MapLayout(width,height);
//     super.setLayout(ml);
//    } 
   
public BorderPanel(Xsocket s,int i,int w,int h,int b,int bc)
     {
     super();
     inner = new BasePanel(s,i,w,h);
     width  = w;
     height = h; 
     border = b;
     borderColour = new Color(bc);
     setBackground(borderColour);
     ml     = new MapLayout(width,height);
     super.setLayout(ml);	// ??????
     add((Component) inner,"1");
     ml.setConstraint(inner,border,border,border+width,border+height);
     layout();
     }

public void resize(int w,int h)
   {
   Trail("Resize");
   super.resize(w+2*border,h+2*border);
   inner.resize(w,h);
   }

private void Trail(String s)
   {
   System.out.println(s);
   }
   
public void paint(Graphics g)
   {
   //Graphics2D g2D = (Graphics2D) g;

   //g2D.setColor(borderColour);
   //g2D.setStroke(new BasicStroke(border));
   //inner.paint(g);
   //g2D.drawRect(0,0,width-border,height-border);
   // g.drawString("Win "+index,5,20);
   }

public Graphics getGraphics()
   {
   return(inner.getGraphics());
   }

public void mapWindow()
   {
   super.setVisible(true);
   inner.setVisible(true);
   }

public void unMapWindow()
   {
   super.setVisible(false);
   inner.setVisible(false);
   }
 
   public void registerChild(Xwindow w)
      {
      Trail("BorderPanel Register Child");
      inner.registerChild(w);
      }

   public void resetChild(Xwindow c)
      {
      Trail("BorderPanel Reset Child");
      inner.resetChild(c);
      }

   public void removeChild(Xwindow w)
      {
      Trail("BorderPanel Remove Child");
      inner.removeChild(w);
      }
   }

