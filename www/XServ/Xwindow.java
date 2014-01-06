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
import java.awt.color.*;

public class Xwindow
   {
   int       		type;
   BorderPanel 		window;
   XkeyBoard		keyboard;
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
   ColourMap		colMap;
   
   public Xwindow(int i,XkeyBoard k,int p,int x,int y,int w,int h)
      {
      type    = 0;
      parent  = p;
      xPos    = x;
      yPos    = y;
      width   = w;
      height  = h;
      index   = i;
      keyboard = k;
      colMap = null;
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
      colMap = null;
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

   public static String hex(int n)
   {
   // call toUpperCase() if that's required
   return String.format("0x%2s", Integer.toHexString(n & 0xFF)).replace(' ', '0');
   }

   public void printByteArray(byte array[],int len)
   {
   int i;
   
   for ( i=0; i < len; i++)
       {
       Trail(hex(array[i]));
       }
   }

   public Graphics getGraphics()
   {
	   if (image != null ) return(image.getGraphics());
	   if ( parent == 0 ) return(frame.getGraphics());
	   return window.getGraphics();
   }

   public Xwindow create(int i,int p,int x,int y,int w, int h ,int b, int bc,int bgc)
   {
      Xwindow win = new Xwindow(i,keyboard,p,x,y,w,h); 
      Trail("New Window parameters : x "+x+" y : "+y+ " border : "+b);
      Trail(" w : "+w+" h : "+h);
      win.border = b;
      if ( p == 0 )
         {
         Trail("Base Window");
         win.createFrame(bgc);
         }
      else
         {
    	 Trail("Sub window");
    	 win.createPanel(b,bc,bgc);
         }
      return(win);
   }
   

   public void createFrame(int bgc)
   {
   frame = new Xframe(index,keyboard,width,height,bgc);
   }
   
   public void createPanel(int b,int bc,int bgc)
   {
   window = new BorderPanel(index,keyboard,width,height,b,bc,bgc);
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

   public void mapWindow()
   {
   Trail("X MapWindow");
   if ( window != null) window.mapWindow();
   }

   public void unMapWindow()
   {
   Trail("X UnMapWindow");
   if ( window != null) window.unMapWindow();
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

   public void setColourMap(ColourMap cm)
   {
   colMap = cm;
   }


   public void putImage(Image img,int x,int y,int w,int h)
   {
      Graphics g = getGraphics();
 
//      g.drawImage(img,x,y,w,h,this);
      repaint();
   }

   public void putImage(byte imgBuff[],int bits,int lp,int x,int y,int w,int h)
   {
      int i;
      byte c = (byte) 0xFF;
      Graphics g;
      BufferedImage img;
      Point pt = new Point(x,y);
           
      Trail("New Data Buffer byte : "+imgBuff.length);
//    Uncomment for debugging
//      for ( i=0; i < imgBuff.length; i++ )
//          {
//	  imgBuff[i] = (byte) (c - imgBuff[i]);
//          }
//      printByteArray(imgBuff,imgBuff.length);
      DataBuffer dBuffer = new DataBufferByte(imgBuff, w * h);
      Trail("Packed Raster - bits : "+bits);    
      if ( bits < 24 )
         {
	 //      WritableRaster wr = Raster.createPackedRaster(dBuffer,w,h,bits,pt);
	 WritableRaster wr = Raster.createPackedRaster(dBuffer,w,h,bits,null);
         Trail("Colour Map : "+colMap.mapId);
         IndexColorModel icm = colMap.returnIndexColModel(bits);
	 img = new BufferedImage(icm, wr, false, null);
         }
      else
	 {
	 Trail("Direct Colour");
         WritableRaster wr = Raster.createBandedRaster(DataBuffer.TYPE_BYTE,w,h,4,null);
         wr.setDataElements(0, 0, w, h, imgBuff);

         ColorSpace sRGB = ColorSpace.getInstance(ColorSpace.CS_sRGB);

         ComponentColorModel ccm = new ComponentColorModel(sRGB, true, false, Transparency.OPAQUE, DataBuffer.TYPE_BYTE);
	 //DirectColorModel dcm = new DirectColorModel(bits,0xFF0000,0xFF00,0xFF);
	 //img = new BufferedImage(dcm, wr, false, null);
         img = new BufferedImage(ccm,wr,false,null);
	 }
      Trail("drawImage "+bits+" w "+w+" h "+h);   
      g = getGraphics();
      g.drawImage(img,x,y,w,h,null);
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
