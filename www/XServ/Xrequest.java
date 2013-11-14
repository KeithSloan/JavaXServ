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

public final class Xrequest extends Xsocket
{
  Xsocket  sock = new Xsocket();
  // What is Base panel doing
  BasePanel base = new BasePanel();
  int      sequenceNum = 0;
  int      majorVersion;
  int      minorVersion;
  String   authProtocol;
  int      IAtoms;  
  int      numWindows;
  int      currentWindow;
  Font     currentFont;
  Font     testFont;
  Xwindow  Windows[] = new Xwindow[450];
  GraphicsContextSet GCSet = new GraphicsContextSet();
  IndexColorModel im,wicm;
  // The following are passed from action to paint 
  int parm;
  int byteLen;		// Length in bytes
  int wordLen;		// Length in 4 bytes words
  int debugX = 0;
  int debugY = 200; 
  byte Reds[] = new byte[256];
  byte Greens[] = new byte[256];
  byte Blues[] = new byte[256];
  int  numColors;

private void Trail(String s)
   {
   System.out.println(s);
   }

public Component init()
   {
   byte r[] = {(byte) 0, (byte) 255};
   im = new IndexColorModel(1,2,r,r,r);
   Trail("Request Init");
   
   //wicm = (IndexColorModel) Windows[0].window.getColorModel();
   //numColors = wicm.getMapSize();
   //Trail("Number of Colour "+numColors);
   //wicm.getReds(Reds);
   //wicm.getGreens(Greens);
   //wicm.getBlues(Blues);
   return((Component) Windows[0].window);
   }

public void setup(String application,String server,int port)
   {
   sequenceNum = 0;
   IAtoms = 0x98;;
   numWindows = 0;
   sock.open(server,port);
   sock.setByteOrder((char) 0x42); 
   sendApplication(application);
   Windows[0] = new Xwindow(sock,0,0,0,0,800,640);
   currentFont = new Font("Courier",Font.PLAIN,10);
   testFont = new Font("Courier",Font.PLAIN,10);
   }

public Panel initBasePanel()
{
	base.init();
	return(base);
}

public void sendApplication(String application)
{
	sock.setByteOrder((char ) 0x6C);
	sock.replyHeader4(100,0,application.length());
	sock.addString(application);
	sock.flushBuffer();
}

public void clearRequest(int len )
   {
   Trail("Clear Bytes : "+len);
   while ( len > 0 )
      {
      sock.readByte(true);
      len--;
      }
   }

private void replyInternAtom(int atom)
   {
   sock.replyHeader4(1,0,sequenceNum);
   sock.addCard32(0);
   sock.addCard32(atom);
   sock.flush();
   }

public void InternAtom(int flag, int len)
   {
   int sl;
   String s;

   Trail("InternAtom flag "+flag);
   sl = sock.readCard16();
   sock.readCard16();
   s  = sock.readString(sl);
   System.out.println(s); 
   clearRequest(len - 8 - sl);
   if ( flag == 1 )
      replyInternAtom(0);
   else
      replyInternAtom(IAtoms++);
   }

public void replyPropertyNo()
   {
   sock.replyHeader4(1,0,sequenceNum);
   sock.addCard32(0);
   sock.addCard32(0);
   sock.addCard32(0);
   sock.flush();
   }

public void replyProperty(String s)
   {
   int sl = s.length();
   int rl = (sl + 3)/4; // String length in 4 byte units
   int p  = 4*rl - sl;

   sock.replyHeader4(1,8,sequenceNum);
   sock.addCard32( 8 + rl);
   sock.addCard32(31);		// Xa String
   sock.addCard32(p);
   sock.addCard32(sl);
   sock.addFill(12);
   sock.addString(s);
   sock.addFill(p);
   sock.flush();
   }

public void GetProperty()
   {
   int win,property,type,offset,length;

   Trail("Get Property");
   win      = sock.readCard32();
   Trail("Window   : "+win);
   property = sock.readCard32();
   Trail("Property : "+property);
   type     = sock.readCard32();
   Trail("Type     : "+type);
   offset   = sock.readCard32();
   Trail("Offset   : "+offset);
   length   = sock.readCard32();
   Trail("length   : "+length);
   switch ( property )
      {
      case 23 :  // Resource Manager ( see /usr/include/X11/Xatom.h
	 replyPropertyNo();
         break;

      default :
         System.out.println("Property not handled  : "+property);
	 replyPropertyNo();
	 break;
      }
   }

public void GetInputFocus()
   {
   sock.replyHeader4(65,2,sequenceNum);
   sock.addCard32(0);
   sock.addCard32(0);   // Window with Focus : For now say root
   sock.addFill(20);
   sock.flush();
   }

public void OpenFont(int rl) // 45
   {
   int fontId = sock.readCard32();
   int sl      = sock.readCard16();
   sock.readCard16();
   Trail("Open Font Id : "+fontId+" : "+sock.readString(sl));
   clearRequest(rl - 12 - sl);
   }

public void QueryFont() // 47
   {
   int fontTable = sock.readCard32();
   int a,d;
   Font fnt;
   // FontMetrics metrics = FontMetrics(fnt);

   Trail("Query Font");
   // Just make up any font for now. 
   fnt = new Font("Courier",Font.PLAIN,16);
   // metrics = new FontMetrics(fnt);
   // a = metrics.getAscent();
   // d = metrics.getDescent();
   a = 7;
   d = 8;

   sock.replyHeader4(1,0,sequenceNum);
   sock.addCard32(7);				// Additional Length
   sock.addCard16(0);				// min left bearing
   sock.addCard16(0);				// min right bearing
   // sock.addCard16(metrics.charWidth('.'));	// min char width
   sock.addCard16(6);

   sock.addCard16(a);				// min char ascent
   sock.addCard16(d);				// min char descent 
   sock.addCard16(0);				// min attributes 
   sock.addFill(4);				// unused
   sock.addCard16(0);				// max left bearing
   sock.addCard16(0);				// max  right bearing
   //sock.addCard16(metrics.charWidth('M'));	// max char width
   sock.addCard16(12);

   sock.addCard16(a);				// max char ascent
   sock.addCard16(d);				// max char descent 
   sock.addCard16(0);				// max attributes 
   sock.addFill(4);				// unused
   sock.addCard16(0);	  			// min char
   sock.addCard16(0);	  			// max char
   sock.addCard16(0);	  			// default Char 
   sock.addCard16(0);  				// Num of font Properties
   sock.addByte(0);				// draw left-right
   sock.addByte(0);				// min Byte 1
   sock.addByte(0); 				// max Byte 1
   sock.addByte(0); 				// All Char Exist 
   sock.addCard16(a);				// Font Ascent
   sock.addCard16(d);				// Font Desecent
   sock.addCard32(0);				// Number of CharInfo's
   sock.flush();
   }

// This is old code Need to update to change entry in colourmap
public void AllocColor()
   {
   int r,g,b;
   int i,m,n,d;

   sock.readCard32();  	// Colour Map
   r = sock.readCard16();
   g = sock.readCard16();
   b = sock.readCard16();
       sock.readCard16();
   Trail("Alloc Colour "+r+" "+g+" "+b);
   n = 0xFFFFFF;
   m = 0;
   for ( i=0; i < numColors; i++ )
       {
       d = (r - Reds[i]) * (g - Greens[i]) * (b - Blues[i]);
       if ( d < 0 ) d = -d;
       if ( d < n )
          {
          m = i;
          n = d;
	  }
       }     
   sock.replyHeader4(1,0,sequenceNum);
   sock.addCard32(0);
   sock.addCard16(Reds[m]); 
   sock.addCard16(Greens[m]);
   sock.addCard16(Blues[m]);
   sock.addCard16(0); 
   sock.addCard32(m);
   sock.addFill(12);
   sock.flush();
   }

public void AllocNamedColor(int len)
   {
   int sl;
   String name;
   Color c = new Color(200,200,0);
   int r,g,b;

   sock.readCard32();  	// Colour Map
   sl = sock.readCard16();
   sock.readCard16();
   name  = sock.readString(sl);
   Trail("Alloc Colour : "+name);
   clearRequest(len - 12 - sl);

   c.getColor(name);
   r = c.getRed();
   g = c.getGreen();
   b = c.getBlue();

   sock.replyHeader4(1,0,sequenceNum);
   sock.addCard32(0);
   sock.addCard32(0);		// Pixel ?
   sock.addCard16(r); 
   sock.addCard16(g);
   sock.addCard16(b);
   sock.addCard16(r); 
   sock.addCard16(g);
   sock.addCard16(b);
   sock.flush();
   }

public void QueryColors(int num)
   {
   int i;
   Trail("Read Colour Map - Colours : "+num);
   sock.readCard32();  	// Colour Map

   sock.replyHeader4(1,0,sequenceNum);
   sock.addCard32(num << 1);	
   sock.addCard16(num);	
   sock.addFill(22);
   while ( num > 0 )
      {
      Trail("Read Pixel");
      i = sock.readCard32();
      sock.addCard16(Reds[i]); 
      sock.addCard16(Greens[i]); 
      sock.addCard16(Blues[i]); 
      sock.addCard16(0); 
      num--;
      }
   sock.flush();
   }

public static String hex(int n)
    {
    // call toUpperCase() if that's required
    return String.format("0x%8s", Integer.toHexString(n)).replace(' ', '0');
    }

public void createGC(int wlen)
    {
    int clearLen,len;
    int cid, drawable, bitmask,function,planeMask,foreGround,backGround;
    int lineWidth,lineStyle,capStyle,joinStyle,fillStyle,fillRule;
    int tile,stipple,stippleXorig,stippleYorig,font;
    int clipXorig,clipYorig,clipMask,dashOffset,dashes,arcMode;
	
    Trail("CreateGC : WordLen "+wlen);
    cid      = sock.readCard32();  	// Graphic Context
    Trail("CreateGC : "+cid);
    drawable = sock.readCard32();
    bitmask  = sock.readCard32();
    Trail("Bit Mask : "+hex(bitmask));
    len = 16;
    foreGround = backGround = lineWidth = lineStyle = font = 0;
    if (( bitmask & 0x00000001) == 0x00000001)
       {
       function = sock.readByte(true);
       Trail("Function : "+function);
       len++;
       }
    if (( bitmask & 0x00000002) == 0x00000002)
       {
       planeMask = sock.readCard32();
       len += 4;
       }
    if (( bitmask & 0x00000004) == 0x00000004)
       {
       foreGround = sock.readCard32();
       len += 4;
       }
    if (( bitmask & 0x00000008) == 0x00000008)
       {
       backGround = sock.readCard32();
       len += 4;
       }
    if (( bitmask & 0x00000010) == 0x00000010)
       {
       lineWidth = sock.readCard16();
       len += 2;
       }
    if (( bitmask & 0x00000020) == 0x00000020)
       {
       lineStyle = sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00000040) == 0x00000040)
       {
       capStyle = sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00000080) == 0x00000080)
       {
       joinStyle = sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00000100) == 0x00000100)
       {
       fillStyle = sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00000200) == 0x00000200)
       {
       fillRule = sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00000400) == 0x00000400)
       {
       tile = sock.readCard32();
       len += 4;
       }
    if (( bitmask & 0x00000800) == 0x00000800)
       {
       stipple = sock.readCard32();
       len += 4;
       }
    if (( bitmask & 0x00001000) == 0x00001000)
       {
       stippleXorig = sock.readCard16();
       len += 2;
       }
    if (( bitmask & 0x00002000) == 0x00002000)
       {
       stippleYorig = sock.readCard16();
       len += 2;
       }
    if (( bitmask & 0x00004000) == 0x00004000)
       {
       font = sock.readCard32();
       len += 4;
       }
    if (( bitmask & 0x00008000) == 0x00008000)
       {
       sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00010000) == 0x00010000)
       {
       sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00020000) == 0x00020000)
       {
       clipXorig = sock.readCard16();
       len += 2;
       }
    if (( bitmask & 0x00040000) == 0x00040000)
       {
       clipYorig = sock.readCard16();
       len += 2;
       }
    if (( bitmask & 0x00080000) == 0x00080000)
       {
       clipMask = sock.readCard32();
       len += 4;
       }
    if (( bitmask & 0x00100000) == 0x00100000)
       {
       dashOffset = sock.readCard16();
       len += 2;
       }
    if (( bitmask & 0x00200000) == 0x00200000)
       {
       dashes = sock.readByte(true);
       len++;
       }
    if (( bitmask & 0x00400000) == 0x00400000)
       {
       arcMode = sock.readByte(true);
       len++;
       }
    Trail("Calc Len : "+len);
    clearLen = ((wlen << 2) - len);
    if (clearLen != 0 ) clearRequest(clearLen);
    Trail("Add to set");
    GCSet.add(cid,bitmask,foreGround,backGround,lineWidth,lineStyle,font);
    }




public int action()
   {
   int parm;
   int byteLen;		// Length in bytes
   int wordLen;		// Length in 4 bytes words
   int opcode;
   int count;
   int jw;
   int gc;
   int shape,m,delta;
   int cx,cy,x,y,h,w,a1,a2;
   int i,p,b,d;
   int xArray[] = new int[1024];
   int yArray[] = new int[1024];
   byte textBuff[] = new byte[257];
   String s;
   Graphics gp,gi;
   Xframe baseFrame;

   Trail("About to read Request - bytes available :"+sock.available());
   opcode = sock.readByte(true);
   parm = sock.readByte(true);
   wordLen = sock.readCard16();
   byteLen = wordLen << 2;  
   Trail("Opcode : "+opcode+" Length bytes:"+byteLen);
   sequenceNum++;

   switch (opcode)
      {
      case 1 :	// Create Window
          p = sock.readCard16();	// parent
	  i = sock.readCard16();
          x = sock.readCard16(); 
          y = sock.readCard16(); 
          w = sock.readCard16(); 
          h = sock.readCard16(); 
          Trail("Create Window Target : "+i+" Parent : "+p);
	  Trail("x "+x+" y "+y+" w "+w+" h "+h);
          if ( p == 0 && w == 1 && h == 1)
             {
             w = 256;
             h = 256;
             }
	  Windows[i] = Windows[p].create(i,p,x,y,w,h);
          Windows[p].registerChild(Windows[i]);
//	      Windows[i].buildImage();
          break;

      case 4 :  // Destroy Window
          i = sock.readCard32();
          Trail("Destroy Window : "+i);
          Windows[i].destroy();
          break;

      case 12 :	// Config Window
	  i = sock.readCard32();
	  p = Windows[i].parentIndex();
          Trail("Config window : "+i);
          if ( p == 0 )
             {
             System.out.println("Config Frame");
             }
          else if ( Windows[i].configWindow() == true )
             {
             Windows[p].resetChild(Windows[i]);
	     }
          break;

      case  16 :
    	  InternAtom(parm,byteLen);
    	  break;

      case  20 :
          GetProperty();
          break;

      case  43 :
          GetInputFocus();
          break;

      case  45 :
    	  OpenFont(byteLen);
    	  break;

      case  47 :
          QueryFont();
          break;

      case  53 :	// Create PixMap
          i = sock.readCard16();
          w = sock.readCard16();
          h = sock.readCard16();
          d = sock.readCard16();
          // Pixmap constructor
          Trail("Create Pixmap : "+i+" w "+w+" h "+h+ " depth "+d);
          Windows[i] = new Xwindow(i,w,h,d);
          break;
          
      case 55 :
    	  createGC(wordLen);
    	  break;
                    
	  case 59 :		// Set Clip Rectangles
          jw = sock.readCard32();
          Trail("Clip Rectangle : "+jw);
          gp =  Windows[jw].getGraphics(); // Only action if valid Windows found
          x  = sock.readCard16();
          y  = sock.readCard16();
          count = (wordLen - 3) / 2;
 	  i = 0;
	  do
	     {
             cx = sock.readCard16();
             cy = sock.readCard16();
             w  = sock.readCard16();
             h  = sock.readCard16();
             gp.clipRect(cx,cy,w,h);
	     i++;
	     } while ( i < count );        
          Trail("Clip Rectangle : "+jw+" x "+cx+" y "+cy+" w "+w+" h "+h);
          break;

      case 61 :		// Clear Area
          jw = sock.readCard32();
          x = sock.readCard16();
          y = sock.readCard16();
          w = sock.readCard16();
          h = sock.readCard16();
          Windows[jw].clearArea(x,y,w,h);
          Trail("Clear Area : "+jw+" x "+x+" y "+y+" w "+w+" h "+h);
          break;

      case 62 :		// CopyArea
          {
          int src,dst,sx,sy,dx,dy;
          Image img;
           
          src = sock.readCard32();
          dst = sock.readCard32();
          gc  = sock.readCard32();
          sx  = sock.readCard16();
          sy  = sock.readCard16();
          dx  = sock.readCard16();
          dy  = sock.readCard16();
          w = sock.readCard16();
          h = sock.readCard16();
          Trail("Copy Area : "+src+" x "+sx+" y "+sy+" w "+w+" h "+h);
          // Trail(" To "+dst+" x "+sx+" y "+sy);
          // img = Windows[src].getImage(sx,sy,w,h);
          // Windows[dst].putImage(img,dx,dy);
	  }
          break;
 
      case 63 :		// CopyPlane
          {
          int src,dst,sx,sy,dx,dy;
          Image img;
           
          src = sock.readCard32();
          dst = sock.readCard32();
          gc  = sock.readCard32();
          sx  = sock.readCard16();
          sy  = sock.readCard16();
          dx  = sock.readCard16();
          dy  = sock.readCard16();
          w   = sock.readCard16();
          h   = sock.readCard16();
          d   = sock.readCard32();
          // Trail("Copy Plane : "+src+" x "+sx+" y "+sy+" w "+w+" h "+h);
          // Trail(" To "+dst+" x "+sx+" y "+sy);
          // img = Windows[src].getImage(sx,sy,w,h);
          // Windows[dst].putImage(img,dx,dy);
	  }
          break;

      case 65 :		// PolyLine
    	  count = wordLen - 3;
    	  jw = sock.readCard32();
    	  Trail("PolyLine : "+parm+" Window : "+jw);
          gp =  Windows[jw].getGraphics();
          //gp.setColor(Color.blue);
	  gp.setXORMode(Color.yellow);
          gc = sock.readCard32();
          Trail("Graphics Context : "+gc);
          Trail("points : "+count);
          cx = sock.readCard16();
          cy = sock.readCard16();
          i = 1;
          switch (parm)
             {
  	     case 0 :	
	          while (  i < count )
	             {
                     x = sock.readCard16();
	             y = sock.readCard16();
	             gp.drawLine(cx,cy,x,y);
	             cx = x;
	             cy = y;
	             i++;
		     }
	         break;

	     case 1 :
	          while (  i < count )
	             {
                     x = cx + sock.readCard16();
	             y = cy + sock.readCard16();
	             gp.drawLine(cx,cy,x,y);
	             cx = x;
	             cy = y;
	             i++;
	             }
	          break;

	     }
          //Windows[jw].repaint();
	  break;

      case 66 : 	// Line segment
    	  jw = sock.readCard32();
          gc = sock.readCard32();
          count = (wordLen - 3) >> 1;
	  Trail("Segment - Window : "+jw+" : "+count);
	  i = 0;
          gp = Windows[jw].getGraphics();
          Trail("Graphic "+gp);
          gp.setColor(Color.black);
          while ( i < count )
             {
             cx = sock.readCard16();
             cy = sock.readCard16();
             x = sock.readCard16();
             y = sock.readCard16();
             gp.drawLine(cx,cy,x,y);
             Trail("x1 "+cx+" y1 "+cy+" x2 "+x+" y2 "+y);
             i++;
	     }
          Windows[jw].repaint();
	  break;

      case 67 :		// PolyRect
          jw = sock.readCard32();
          gc = sock.readCard32();
          count = (wordLen - 3) >> 1;
          Trail("Segment - Window : "+jw+" : "+count);
          i = 0;
          gp = Windows[jw].getGraphics();
          gp.setColor(Color.black);
          while ( i < count )
             {
             x = sock.readCard16();
             y = sock.readCard16();
             w = sock.readCard16();
             h = sock.readCard16();
             gp.drawRect(x,y,w,h);
             i++;
             }
          Windows[jw].repaint();
          break; 


      case 69 :	// PolyFill
    	  count = wordLen - 4;
    	  jw = sock.readCard32();
    	  Trail("FillPoly - window "+jw);
          gc = sock.readCard32();
          shape = sock.readByte(false);
    	  m = sock.readByte(false);
    	  sock.readCard16();
    	  gp = Windows[jw].getGraphics();
    	  gp.setColor(Color.yellow);
    	  Trail("Mode : "+m+" Shape : "+shape+" GC : "+gc);
          if ( count < 1024 )
             {
             switch ( m )
        	{
        	case 0 :
        		i = 0;
        		while ( i < count )
        	           {	
        	 	   xArray[i] = sock.readCard16();
        	 	   yArray[i] = sock.readCard16();
        	 	   i++;
        	 	   }
        	 	break;

        	case 1 :
        		i = 1;
        		x = xArray[0] = sock.readCard16();
        		y = yArray[0] = sock.readCard16();
        		while ( i < count )
        	           {
        	 	   x += sock.readCard16();
        	 	   y += sock.readCard16();
        	 	   xArray[i] = x;
        	 	   yArray[i] = y;
        	 	   i++;
        	 	   }
        	 	break;
        	 }
        	 gp.fillPolygon(xArray,yArray,count);
             }
          else
             {
             System.out.println("Too many points : "+count);
             clearRequest(count << 2 );
             }
          Windows[jw].repaint();
          break;

      case 70 :		// PolyFill Rectangle
    	  jw = sock.readCard32();
    	  gp = Windows[jw].getGraphics();
    	  Trail("PolyFill Rectangle - Window : "+jw);
          gc = sock.readCard32();
          count = (wordLen - 3) >> 1;
          gp.setColor(Color.green);
          gp.setColor(Color.red);
          while ( count > 0 )
             {
             xArray[0] = xArray[3] = sock.readCard16();
             yArray[0] = yArray[1] = sock.readCard16();
             xArray[1] = xArray[2] = sock.readCard16();
             yArray[2] = yArray[3] = sock.readCard16();
             gp.fillPolygon(xArray,yArray,4);
             count--;
             }	
          Windows[jw].repaint();
          break;

      case  71 :   // PolyFillArc
    	  count = (wordLen - 3) / 3;
    	  jw = sock.readCard32();
    	  Trail("PolyFillArc : "+parm+" Window : "+jw);
          gp =  Windows[jw].getGraphics();
          gp.setColor(Color.blue);
          gc = sock.readCard32();
          Trail("num of arcs : "+count);
          i = 0;
          while ( i < count )
             {
             x = sock.readCard16();
             y = sock.readCard16();
             w = sock.readCard16();
             h = sock.readCard16();
             a1 = sock.readCard16();
             a2 = sock.readCard16();
             gp.fillArc(x + w/2,y+h/2,w,h,a1,a2);
             i++;
             } 
          Windows[jw].repaint();
          break;

      case 72 :		// PutImage
          {
          byte  imageBuff[];

          Trail("PutImage format : "+parm);
          jw = sock.readCard32();
          gc = sock.readCard32();
          w = sock.readCard16();
          h = sock.readCard16();
          x = sock.readCard16();
          y = sock.readCard16();
          p = sock.readByte(false);	// Padding
          b = sock.readByte(false);
          Trail("Window : "+jw+ " Bits : "+b+" Left Padding "+p);
	  sock.readCard16();		// Unused 
          i = byteLen - 24;
          Trail("PutImage : "+jw+" x "+x+" y "+y+" w : "+w+" h : "+h+" size "+i);
          imageBuff = new byte[i];
          Trail("Bytes Read "+sock.readBuff(imageBuff,i)+" len "+i);
          Windows[jw].putImage(imageBuff,b,p,x,y,w,h);
          // Windows[0].drawImage(Windows[jw].getImage(),debugX,debugY,100,100);
          debugX += 100;
          }
          break;

      case 74 :		// PolyText
    	  jw = sock.readCard32();
    	  gp = Windows[jw].getGraphics();
    	  Trail("PolyText8 - Window "+jw);
          gc = sock.readCard32();
          x = sock.readCard16();
          y = sock.readCard16();
          i = sock.readByte(false);
          Trail("String len "+i);
          delta = sock.readByte(false);
          gp.setColor(Color.black);
          gp.setFont(currentFont);
          if ( i < 256 )
	     {
             sock.readBuff(textBuff,i);
             textBuff[i] = 0;
             Trail("String : "+textBuff+" "+x+" "+y);
             // Old Problem with drawBytes
             // gp.drawBytes(textBuff,0,i,x,y);
             s = new String(textBuff);
             gp.drawString(s,x,y);
             }
          else
	     {
             System.out.println("PolyText8 font shift : Help!");
             i = 0;
	     }
          // delete repaint ???
          Windows[jw].repaint();
          clearRequest(byteLen - 18 - i);
          break;

      case  84 :
    	  AllocColor();
    	  break;   
 
      case  85 :
    	  AllocNamedColor(byteLen);
    	  break;

      case  91 :
    	  QueryColors(wordLen - 2);
    	  break;

      case 127 :
    	  System.out.println("No Op");
    	  break;

      default : // Op code for which I have no code yet
    	  printOperation(opcode);
      	  clearRequest(byteLen - 4);
      	  break;
      }
   return(0);
   }

public void printOperation(int opcode)
   {
   switch ( opcode )
      {
      case 1 :
              Trail("CreateWindow");
              break;

      case 2 :
              System.out.println("Change Window Attributes");
              break;

      case 3 :
              System.out.println("Get Window Attributes");
              break;

      case 4 :
              System.out.println("DestroyWindow");
              break;

      case 5 :
              System.out.println("Destroy Sub Windows");
              break;

      case 6 :
              System.out.println("Change Save Set");
              break;

      case 7 :
              System.out.println("ReparentWindow");
              break;

      case 8 :
              Trail("MapWindow");
              break;

      case 9 :
              Trail("MapSubwindows");
              break;

      case 10 :
              Trail("UnmapWindow");
              break;

      case 11 :
              System.out.println("UnmapSubWindow");
              break;

      case 12 :
              Trail("ConfigureWindow");
              break;

      case 13 :
              System.out.println("CirculateWindow");
              break;

      case 14 :
              System.out.println("Get Gemometry");
              break;

      case 15 :
              System.out.println("Query Tree");
              break;

      case 16 :
              Trail("InternAtom");
              break;

      case 17 :
              System.out.println("GetAtomName");
              break;

      case 18 :
              Trail("ChangeProperty");
              break;

      case 19 :
              System.out.println("DeleteProperty");
              break;

      case 20 :
              Trail("GetProperty");
              break;

      case 21 :
              System.out.println("ListProperty");
              break;

      case 22 :
              System.out.println("SetSelectionOwner");
              break;

      case 23 :
              System.out.println("GetSelection");
              break;

      case 24 :
              System.out.println("ConvertSelection");
              break;

      case 25 :
              System.out.println("SendEvent");
              break;

      case 26 :
              System.out.println("GrabPointer");
              break;

      case 27 :
              System.out.println("UnGrabPointer");
              break;

      case 28 :
              System.out.println("GrabButton");
              break;

      case 29 :
              System.out.println("UnGrabButton");
              break;

      case 30 :
              System.out.println("ChangeActivePointerGrab");
              break;

      case 31 :
              System.out.println("GrabKeyBoard");
              break;

      case 32 :
              System.out.println("UnGrabKeyBoard");
              break;

      case 33 :
              System.out.println("GrabKey");
              break;

      case 34 :
              System.out.println("UnGrabKey");
              break;

      case 35 :
              System.out.println("AllowEvents");
              break;

      case 36 :
              System.out.println("GrabServery");
              break;

      case 37 :
              System.out.println("UnGrabServer");
              break;

      case 38 :
              System.out.println("QueryPointer");
              break;

      case 39 :
              System.out.println("GetMotionEvents");
              break;

      case 40 :
              System.out.println("TranslateCoordinates");
              break;

      case 41 :
              System.out.println("WarpPointer");
              break;

      case 42 :
              System.out.println("SetInputFocus");
              break;

      case 43 :
              Trail("GetInputFocus");
              break;

      case 44 :
              System.out.println("QueryKeymap");
              break;

      case 45 :
              Trail("OpenFont");
              break;

      case 46 :
              System.out.println("CloseFont");
              break;

      case 47 :
              Trail("QueryFont");
              break;

      case 48 :
              System.out.println("QueryFontExtents");
              break;

      case 49 :
              Trail("ListFonts");
              break;

      case 50 :
              Trail("ListFontsWithInfo");
              break;

      case 51 :
              System.out.println("SetFontPath");
              break;

      case 52 :
              System.out.println("GetFontPath");
              break;

      case 53 :
              Trail("CreatePixMap");
              break;

      case 54 :
              System.out.println("FreePixMapSet");
              break;

      case 55 :
              System.out.println("CreateGC");
              break;

      case 56 :
              System.out.println("ChangeGC");
              break;

      case 57 :
              System.out.println("CopyGC");
              break;

      case 58 :
              System.out.println("SetDashes");
              break;

      case 59 :
              System.out.println("SetClipRectangles");
              break;

      case 60 :
              System.out.println("FreeGC");
              break;

      case 61 :
              Trail("ClearArea");
              break;

      case 62 :
              System.out.println("CopyArea");
              break;

      case 63 :
              System.out.println("CopyPlane");
              break;

      case 64 :
              System.out.println("PolyPoint");
              break;

      case 65 :
              Trail("PolyLine");
              break;

      case 66 :
              Trail("PolySegment");
              break;

      case 67 :
              Trail("PolyRectangle");
              break;

      case 68 :
              System.out.println("PolyArc");
              break;

      case 69 :
              Trail("FillPoly");
              break;

      case 70 :
              Trail("PolyFillRectangle");
              break;

      case 71 :
              System.out.println("PolyFillArc");
              break;

      case 72 :
              System.out.println("PutImage");
              break;

      case 73 :
              System.out.println("GetImage");
              break;

      case 74 :
              Trail("PolyText8");
              break;

      case 75 :
              System.out.println("PolyText16");
              break;

      case 76 :
              System.out.println("ImageText8");
              break;

      case 77 :
              System.out.println("ImageText16");
              break;

      case 78 :
              System.out.println("CreateColorMap");
              break;

      case 79 :
              System.out.println("FreeColorMap");
              break;

      case 80 :
              System.out.println("CopyColorMap&Free");
              break;

      case 81 :
              System.out.println("InstallColorMap");
              break;

      case 82 :
              System.out.println("UnInstallColorMap");
              break;

      case 83 :
              System.out.println("ListInstalledColorMap");
              break;

      case 84 :
              System.out.println("AllocColor");
              break;

      case 85 :
              System.out.println("AllocNamedColor");
              break;

      case 86 :
              System.out.println("AllocColorCells");
              break;

      case 87 :
              System.out.println("AllocColorPlanes");
              break;

      case 88 :
              System.out.println("FreeColors");
              break;

      case 89 :
              System.out.println("StoreColor");
              break;

      case 90 :
              System.out.println("StoreNamedColor");
              break;

      case 91 :
              System.out.println("QueryColors");
              break;

      case 92 :
              System.out.println("LookupColor");
              break;

      case 93 :
              System.out.println("CreateCursor");
              break;

      case 94 :
              System.out.println("CreateGlyphCursor");
              break;

      case 95 :
              System.out.println("FreeCursor");
              break;

      case 96 :
              System.out.println("ReColorCursor");
              break;

      case 97 :
              System.out.println("QueryBestSize");
              break;

      case 98 :
              Trail("QueryExtension");
              break;

      case 99 :
              System.out.println("ListExtensions");
              break;

      case 100 :
              System.out.println("ChangeKeyboardMapping");
              break;

      case 101 :
              System.out.println("GetKeyboardMapping");
              break;

      case 102 :
              System.out.println("ChangeKeyboardControl");
              break;

      case 103 :
              System.out.println("GetKeyboardControl");
              break;

      case 104 :
              System.out.println("Bell");
              break;

      case 105 :
              System.out.println("ChangePointerControl");
              break;

      case 106 :
              System.out.println("GetPointerControl");
              break;

      case 107 :
              System.out.println("SetSreenSaver");
              break;

      case 108 :
              System.out.println("GetSreenSaver");
              break;

      case 109 :
              System.out.println("ChangeHosts");
              break;

      case 110 :
              System.out.println("ListHosts");
              break;

      case 111 :
              System.out.println("SetAccessControl");
              break;

      case 112 :
              System.out.println("SetCloseDownMode");
              break;

      case 113 :
              System.out.println("Kill Client");
              break;

      case 114 :
              System.out.println("RotateProperties");
              break;

      case 115 :
              System.out.println("ForceScreenSaver");
              break;

      case 116 :
              System.out.println("SetPointerMapping");
              break;

      case 117 :
              System.out.println("GetPointerMapping");
              break;

      case 118 :
              System.out.println("SetModifierMapping");
              break;

      case 119 :
              System.out.println("GetModifierMapping");
              break;

      case 127 :
              System.out.println("NoOp");
              break;

      case 129 :
              Trail("Shape Extension");
	      break; 

      default :
              System.out.println("Unknown Request :"+opcode);
              break;
      }
   }

   // No longer used
   public boolean connectionInit()
      {
      int lenAuthName;
      int lenAuthData;
      int unused;

      Trail("Connection Init");
      // System.out.println("Bytes Available : "+available());
      sock.setByteOrder(sock.readByte(true));
      unused       = sock.readByte(true);
      majorVersion = sock.readCard16();
      minorVersion = sock.readCard16();
      lenAuthName  = sock.readCard16();
      lenAuthData  = sock.readCard16();
      unused = sock.readCard16();
      sock.readString(lenAuthName);
      sock.readString(lenAuthData);
      Trail("Order : "+byteOrder);
      Trail(" Version :"+majorVersion+" Minor "+minorVersion);
      Trail(" Auth lengths :"+lenAuthName+" len "+lenAuthData);
      return(true);
      }

   // No longer used
   public void sendAcceptAuth()
      {
      sock.replyHeader8(1,majorVersion,minorVersion);
      sock.addCard32(1);                 // Release Number
      sock.addCard32(16);                // Resource-id-base
      sock.addCard32(0xFFFF);            // Resource-id-mask
      sock.addCard32(0);                 // Motion Buffer Size
      sock.addCard16(12);                // Length of Vendor
      sock.addCard16(1024);              // Maximum Request Length
      sock.addByte(1);                   // Number of Screens (n)
      sock.addByte(1);                   // Number of Formats
      sock.addByte(0);                   // Image-byte-Order (0 LSB first)
      sock.addByte(0);                   // Bitmap-Bit-Order (0 LSB first)
      sock.addByte(8);                   // bitmap scan line units
      sock.addByte(32);                  // Bitmap scan line padding
      sock.addByte(0x08);                // min keycode
      sock.addByte(0xFF);                // max keycode
      sock.addCard32(0);                 // Unused
      sock.addString("Keith  Sloan");    // Vendor (v)
                                     // List of Formats (8n)
                                     // List of Screen (m) mutiple of 4
      addFormat(1,0);
      addScreen();
      sock.flushAuth();
      }
       
    // No longer used
    public void addFormat(int d,int b)
      {
      sock.addByte(1);                      // depth
      sock.addByte(8);                      // bits per pixel
      sock.addByte(32);                     // scanline-padding to
      sock.addByte(0);                      // unused
      sock.addCard32(0);                    // unused
      }

    // No longer used
    public void addVisual(int v)
      {
      sock.addCard32(v);                    // Visual Id
      sock.addByte(2);                      // class ( 2 - Static Colour )
      sock.addByte(8);                      // bits per RGB
      sock.addCard16(0);                    // Colour Map Entries
      sock.addCard32(0);                    // Red - Mask
      sock.addCard32(0);                    // Green - Mask
      sock.addCard32(0);                    // Blue - Mask
      sock.addCard32(0);                    // Unused
      }
       
    // No longer used
    public void addDepth(int depth,int nos)
      {
      sock.addByte(depth);                  // depth
      sock.addByte(0);                      // unused
      sock.addCard16(nos);                  // number of visuals
      sock.addCard32(0);                    // unused
      }

    // No longer used
    public void addScreen()
      {
      sock.addCard32(0);                    // window - root
      sock.addCard32(0);                    // default colour Map
      sock.addCard32(0xFFFFFF00);           // white pixel
      sock.addCard32(0x00);                 // black pixel
      sock.addCard32(0xFFFFFFFF);           // Input Mask ( allow all events )
      sock.addCard16(800);                  // Width in pixels
      sock.addCard16(600);                  // Height in Pixels
      sock.addCard16(800);                  // Width in Millimeters
      sock.addCard16(600);                  // Height in Millimeters
      sock.addCard16(0);                    // Min installed maps
      sock.addCard16(0);                    // Max installed maps
      sock.addCard32(0);                    // root-visuals
      sock.addByte(0);                      // Backing Stores (0 - Never)
      sock.addByte(0);                      // save unders ( 0 FALSE )
      sock.addByte(0);                      // root depth
      sock.addByte(0);                      // number of depths
      }  
}
