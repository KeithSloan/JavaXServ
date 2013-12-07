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

public class Xwindow
   {
   int       		type;
   BorderPanel 		window;
   Xsocket   		sock;
   Xframe    		frame;
   BufferedImage	image;
   int       		parent;		// needed for Reconfigure Window 
   int	     		index;	
   int       		xPos;
   int       		yPos;
   int       		width;
   int       		height;
   int       		border;
   int			depth;
   byte 		Reds[] = new byte[256];
   byte 		Greens[] = new byte[256];
   byte 		Blues[] = new byte[256];

   public Xwindow(Xsocket s,int i,int p,int x,int y,int w,int h)
      {
      type    = 0;
      parent  = p;
      xPos    = x;
      yPos    = y;
      width   = w;
      height  = h;
      index   = i;
      sock    = s;
      window = null;
      frame  = null;
      image   = null;
      window  = null;
      } 

     // Pixmap Constructor have to pass an on screen component i.e. root win 
     //   public Xwindow(Component c,int i,int p,int w,int h)
   public Xwindow(int i,int w,int h,int d)
      {
      int type;
	  
      Trail("Pixmap constructor");
      type   = 1;
      index  = i; 
      parent = 0;
      width  = w;
      height = h;
      depth  = d;
      switch (d)
      	{
      	case 1 :
      	case 2 :
      	case 4 :
      		type = 	BufferedImage.TYPE_BYTE_BINARY;
      		break;
      		
      	case 8 :
      		type = BufferedImage.TYPE_BYTE_INDEXED;
      		break;
      	}
      image = new BufferedImage(w,h,type);
      }

   private void Trail(String s)
   {
   System.out.println(s);
   }

   public Graphics getGraphics()
   {
	   if (image != null ) return(image.getGraphics());
	   if ( parent == 0 ) return(frame.getGraphics());
	   return window.getGraphics();
   }

   public Xwindow create(int i,int p,int x,int y,int w, int h ,int b, int bc)
   {
      Xwindow win = new Xwindow(sock,i,p,x,y,w,h); 
      Trail("New Window parameters : x "+x+" y : "+y+ " border : "+b);
      Trail(" w : "+w+" h : "+h);
      win.border = b;
      if ( p == 0 )
         {
         Trail("Base Window");
         win.createFrame();
         }
      else
         {
    	 Trail("Sub window");
    	 win.createPanel(b,bc);
         }
      return(win);
   }
   

   public void createFrame()
   {
   frame = new Xframe(width,height);
   }
   
   public void createPanel(int b,int bc)
   {
   window = new BorderPanel(sock,index,width,height,b,bc);
   }

   public void removeFrame()
   {
   if ( frame != null ) frame.dispose();
   }
   
   public void remove()
   {
   window.removeAll();
   }
   
   public void destroy()
   {
   if ( parent == 0 ) 
      {
      removeFrame();
      }
   else
      {
      window.removeAll();
      }
   }

   public void setXpos(int x)
   {
   xPos = x;	
   }
   
   public void setYpos(int y)
   {
   yPos = y;
   }

   public void setWidth(int w)
   {
   width = w;
   }

   public void setHeight(int h)
   {
   height = h;
   }

   public void registerChild(Xwindow w)
   {
   Trail("X Window RegisterChild");
	  // if ( index != 0 )
	  //    {
   if ( frame != null ) frame.registerChild(w);
   if ( window != null) window.registerChild(w);
	  //    }
	  // return;
   }
   
   public void resetChild(Xwindow w)
   {
   Trail("X Window Reset Child");
   if ( frame != null ) frame.resetChild(w);
   if ( window != null) window.resetChild(w);
   return;
   }

   public void removeChild(Xwindow w)
   {
   Trail("X Window Remove Child");
   window.removeChild(w);
   }
   
   public int parentIndex()
   {
   Trail("X Window Parent Index : "+parent);
   return(parent);
   }
   
   public void repaint()
   {
   Trail("Xwindow Repaint ???");
   }
   public void clearArea(int x, int y, int w, int h)
   {
      Graphics ga = getGraphics();

      Trail("Graphics : "+ga);
      if ( w == 0 ) w = width;
      if ( h == 0 ) h = height;
      Trail("Clear Area : type "+type+" index :"+index +" : "+x+" : "+y+" : "+w+" : "+h); 
      ga.clearRect(x,y,w,h);
      repaint();
   }


   public void putImage(Image img,int x,int y,int w,int h)
   {
      Graphics g = getGraphics();
 
//      g.drawImage(img,x,y,w,h,this);
      repaint();
   }

   public void putImage(byte imgBuff[],int bits,int lp,int x,int y,int w,int h)
   {
      Graphics g;
      BufferedImage img;
      Point pt = new Point(x,y);
           
      DataBuffer dBuffer = new DataBufferByte(imgBuff, width * height);
      WritableRaster wr = Raster.createPackedRaster(dBuffer,width,height,bits,pt);
      IndexColorModel cm = new IndexColorModel(bits,bits,Reds,Greens,Blues);
      img = new BufferedImage(cm, wr, false, null);
      
      Trail("drawImage "+bits+" w "+w+" h "+h);   
      g = getGraphics();
      g.drawImage(img,0,0,w,h,null);
      repaint();
   }

   public Image getImage(int x,int y,int w,int h)
   {
	   Trail("getImage");
 //     return(createImage(new FilteredImageSource(image.getSource()
 //                                ,new CropImageFilter(x,y,w,h))));
	   return((Image) image);
   }

   // Used for debugging
   public Image getImage()
   {
      return(image);
   }
}
