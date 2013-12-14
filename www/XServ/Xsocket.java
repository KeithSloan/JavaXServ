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
import java.net.*;
import java.io.*;


// Need to deal with the fact that length in multiples of 4 bytes
// Add data then set length
public class Xsocket
   {
   SocketPermission		sp;
   ServerSocket			serverSock;
   Socket				sock;
   BufferedInputStream  BuffIn;
   BufferedOutputStream BuffOut;
   DataInputStream  	sockIn;
   DataOutputStream 	sockOut;
   int          		byteOrder;
   byte		       		buffer[];
   int          		insertPoint;
   Trace				trace = new Trace();

   public Xsocket()
      {
      Trail("Buffer Init");
      buffer = new byte[1024];
      }

   private void Trail(String s)
      {
      // System.out.println(s);
      }

   public void setByteOrder(int b)
      {
      byteOrder = b;
      }

   public boolean listen(int port)
      {
      boolean connected;
      trace.On(0xFF);
      connected = false;
      try
         {
         trace.Entry(0x02,"Listening on port : "+port);
         serverSock    = new ServerSocket(port,9000);
    	 // Hack round Interrupted exception
	     do
            {
            try
                {
            	sock = serverSock.accept();
            	connected = true;
            	// Change to do one try as per open function
            	try
            	    {
            		BuffIn = new BufferedInputStream(sock.getInputStream(),4096);
            		sockIn = new DataInputStream(BuffIn);
            	    }
            	catch ( IOException err )
            		{
            		System.out.println("Error mapping to input stream"+err);
            		System.exit(1);
            		}
            	try
            		{
            		BuffOut = new BufferedOutputStream(sock.getOutputStream());
            		sockOut = new DataOutputStream(BuffOut);
            		}
            	catch ( IOException err )
            		{
            		System.out.println("Error mapping to Output stream"+err);
            		System.exit(1);
            		}
                }
            catch ( IOException err )
            	{
            	System.out.println("Error on accept: "+err);
            	connected = false;
            	}
            }
         	while ( connected == false ); 
         }
      catch ( IOException err )
         {
         System.out.println("Error opening socket: "+err);
         System.exit(1);
         }
      return(true);
      }

    public boolean open(String host, int port)
      {
      trace.Entry(0x02,"open port host: "+host+" port : "+port);
      buffer = new byte[4096];
      sp = new SocketPermission(host+":" + port, "connect");

      try
         {
         sock    = new Socket(host,port);
         BuffIn = new BufferedInputStream(sock.getInputStream(),4096);
         sockIn = new DataInputStream(BuffIn);
         BuffOut = new BufferedOutputStream(sock.getOutputStream());
         sockOut = new DataOutputStream(BuffOut);
         }
      catch ( IOException err )
         {
         System.out.println("Error opening socket: "+err);
         return(false);
         }          
      return true;
      }      

    public void close()
      {
      if ( sock != null )
         {
         try
            {
	    trace.Entry(0x02,"Closing Socket");
            sock.close();
	    }
         catch ( IOException err )
            {
            System.out.println("Error Closing socket: "+err);
            }
         sock = null;
         }
      }

   public int available()
      {
      int r = 0;

      try 
         {
         r = sockIn.available(); 
         }
      catch ( IOException err )
         {
         System.out.println("Error reading Socket "+err);
         }
      return(r);
      }
   
   
   // Not sure why need flag
   public int readByte(boolean flag)
      {
      int v = 0;

      if ( sock != null )
      	 {
         try
            {
            v = sockIn.readUnsignedByte();
            }
         catch ( IOException err )
            {
            System.out.println("Error reading Socket "+err);
            System.exit(2);
            }
         if ( flag == true ) 
            {
            // System.out.print("Byte Read : "+v+" ");
        	if ( v != -1 ) trace.Hex(0x01,v);
        	// System.out.println("");
            }
         	if ( v == -1 ) 
         	   {
         	   close();
         	   System.exit(2);
         	   }
         	}
      return(v);
      }

   public int readCard16()
      {
      int r;

      int a = readByte(false);
      int b = readByte(false);
      if ( byteOrder == 66 )  // Octal 102
         r = (( a << 8 ) | b);
      else
         r = (( b << 8 ) | a);
      trace.Entry(0x01,"Read 16 bit "+r);
      return(r);
      }

   public int readCard32()
      {
      int r;
      int a = readByte(false);
      int b = readByte(false);
      int c = readByte(false);
      int d = readByte(false);
      if ( byteOrder == 66 ) // Octal 102
         r = (( a << 24) | ( b << 16) | ( c << 8 ) | d);
      else
         r = (( d << 24) | ( c << 16) | ( b << 8 ) | a);
      trace.Entry(0x01,"Read 32 bit "+r);
      return(r);
      }

   public int readBuff(byte[] buff,int len)
      {
      int i = -1;

      trace.Entry(0x01,"Read bytes length : "+len);

      if ( len > 0 )
         {
         try
            {
            i = sockIn.read(buff,0,len);
            }
         catch ( IOException err )
            {
            System.out.println("Error reading Socket "+err);
            System.exit(2);
            }
         }
      return(i);
      }

   public String readString(int len)
      {
      String s;

      trace.Entry(0x01,"Read String length : "+len);
      if ( len > 0 )
         {
         byte buff[] = new byte[len];
         readBuff(buff,len);
         s = new String(buff,0,len);
         }
      else
         {
    	 s = new String("");
         }
      return(s); 
      }

   public void addByte(int v)
      {   
      trace.Entry(0x01,"Add Byte : "+v);

      buffer[insertPoint++] = (byte) v;
      }  

   public void addCard16(int v)
      {
      trace.Entry(0x01,"Add 16 bit : "+v);
      switch ( byteOrder )
         {
         case 0x42 :
              buffer[insertPoint++] = (byte)( v / 256 );
	          buffer[insertPoint++] = (byte)( v % 256 );
	          break;

         case 0x6C :
	          buffer[insertPoint++] = (byte)( v % 256 );
              buffer[insertPoint++] = (byte)( v / 256 );
	          break;

         default :
	          System.out.println("Invalid byte order"+byteOrder);
    	 }
      }

   public void addCard32(int v)
      {
      int  i;

      trace.Entry(0x01,"Add 32 bit : "+v);
      // System.out.println(insertPoint+" Add 32 bit"+v);
      switch ( byteOrder )
         {
         case 0x42 :
        	 // Insert point goes 3,2,1,0
        	 insertPoint += 3;
        	 for ( i = 0; i < 4; i++)
	          	 {
                 // System.out.println(insertPoint + " MSB first "+ v % 256 );
        		 buffer[insertPoint--] = (byte)( v % 256 );
        		 v = v / 256;
	          	 }
             insertPoint += 5;
             break;
	 
         case 0x6C :
        	 for ( i = 0; i < 4; i++)
        	 	 {
        		 buffer[insertPoint++] = (byte)( v % 256 );
        		 v = v / 256;
        	 	 }
             break;

         default :
        	 System.out.println("Invalid byte order"+byteOrder);
	     }
      }

   public void addString(String s)
      {
      int  len  = s.length();

      trace.Entry(0x01,"Adding String : "+s);
      trace.Entry(0x01,"length : "+len);
      System.arraycopy((Object) s.getBytes(),0,(Object) buffer,insertPoint,len);
      insertPoint += len;
      }
   
   public void addFill(int l)
      {
      while ( l > 0 )
          {
    	  buffer[insertPoint++] = (byte) 0;
          l--;
          }
      }

   public void replyHeader8(int s,int major,int minor)
      {
      trace.Entry(0x04,"Reply header 8 bytes");
      insertPoint = 0;
      addByte(s);
      addByte(0);
      addCard16(major);
      addCard16(minor);
      addCard16(0);
      }

   public void replyHeader4(int r,int p,int seq)
      {
      trace.Entry(0x04,"Reply header 4 bytes");
      insertPoint = 0;
      addByte(r);
      addByte(p);
      addCard16(seq);
      }

   public void replyEvent(int e)
      {
      trace.Entry(0x04,"Reply Event - 32 bytes");
      insertPoint = 0;
      addByte(e);
      }

   public void flushAuth()
      {
      int l = insertPoint;
      trace.Entry(0x04,"Flush : length : "+l);
      if (( l % 4 ) != 0 )
	     {
         System.out.println("Not 4 bytes aligned");
         System.exit(2);
	     }
      insertPoint = 6;
      addCard16((l - 8) >> 2);
      try
         {
         sockOut.write(buffer,0,l);
         sockOut.flush();
         }
      catch ( IOException err )
         {
         System.out.println("Error writing Socket "+err);
         System.exit(2);
         }
      System.out.println("Written");
      }

    public void flushBuffer()
        {
    	int l = insertPoint;
    	try
        {
        sockOut.write(buffer,0,l);
        sockOut.flush();
        }
     catch ( IOException err )
        {
        System.out.println("Error writing Socket "+err);
        System.exit(2);
        }
     trace.Entry(0x04,"Written");
     }
    
    public void flush()
      {
      int a;
      int l = insertPoint;

      if ( l < 32 )
         {
         addFill(32 - l );
         l = 32;
         }
      trace.Entry(0x04,"Flush : length : "+l);
      if (( l % 4 ) != 0 )
      	 {
         System.out.println("Not 4 byte aligned");
         System.exit(2);
      	 }
      insertPoint = 4;
      a = ((l - 32) >> 2);
      trace.Entry(0x04,"Additional Length : "+a);
      addCard32(a);
      try
         {
         sockOut.write(buffer,0,l);
         sockOut.flush();
         }
      catch ( IOException err )
         {
         System.out.println("Error writing Socket "+err);
         System.exit(2);
         }
      trace.Entry(0x04,"Written");
      }

    public void flushEvent()
      {
      int l = insertPoint;

      if ( l < 32 )
         {
         addFill(32 - l );
         }
      l = 32;
      trace.Entry(0x04,"Flush : length : "+l);
      try
         {
         sockOut.write(buffer,0,l);
         sockOut.flush();
         }
      catch ( IOException err )
         {
         System.out.println("Error writing Socket "+err);
         }
      }

   }
