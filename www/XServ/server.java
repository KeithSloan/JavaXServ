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
import java.applet.*;


// NetworkClient has PrintStream as private so cannot see how to
// access

// public class server extends Applet implements Runnable
public class server extends Applet implements Runnable
   {
   private static final long serialVersionUID = 1L;
   static    Xrequest   request = new Xrequest();
   Panel     panel;
   Thread    sockThread;
   int       port = 6789;
   int       graphicOp;
   Font      fnt;
   Trace     trace = new Trace();
   boolean   initFlag = false;
   MapLayout ml    = new MapLayout(800,640);
   String    serverName;
   String    application;

   public server()
      {
      }

   public String getAppletInfo()
      {
      return("JavaXserv version Alpha 1.0 by Keith Sloan keith@sloan-home.co.uk");
      }

   public String[] [] getParameterInfo()
      {
      String pInfo[] [] = {
    	{"application","The X Client Application to Launch"},
    	{"server","text","Host with X Client"},
        {"port","number","port number (default 888)"},	    
        };
      return(pInfo);
      }

   private static void Trail(String s)
      {
      System.out.println(s);
      }

   public void init()
      {
      // Component c;
      String    parmStr;
      Integer parmInt = new Integer(0);

      Trail("TestAppl: Init");
      if ((application = getParameter("application")) == null)
    	 application = "xcalc";
      if ((serverName = getParameter("server")) == null )
         serverName = getDocumentBase().getHost();
      	 if (serverName == "")
      		 serverName ="127.0.0.1";
      if ((parmStr = getParameter("port")) != null )
         port = parmInt.parseInt(parmStr);
      Trail("Server : "+serverName+" Port : "+port);
      }

   public void paint(Graphics g)
      {
      Trail("TestAppl: Start Paint ");
      Trail("TestAppl: End Paint ");
      }

   public void handleKeyPress(char key,boolean isAscii,int mod)
      {
      System.out.println("TestAppl: handle Key Press (" + key + ")");
      }

   public  boolean keyDown(Event e,int key)
      {
      System.out.println("TestAppl: keyDown(" + key + ")");
      return(true);
      }

   public synchronized void update(Graphics g)
      {
      int i;
      // if ( graphicOp != 0 )
      //   {
         // request.paint(g,graphicOp);
       //  request.update(g,graphicOp);
       //  sockThread.resume();
       //  }
      }

   public void start()
      {
      Trail("TestAppl: Starting ");
      if ( sockThread == null )
         {
         sockThread = new Thread(this);
         sockThread.start();
         }
      }

   public void run()
      {
      boolean flg = false;
      Trail("TestAppl: Running");
      request.setup(application,serverName,port);
      while ( flg == false )
         {
         if (( graphicOp = request.action()) != 0 )
            {
            repaint();
            sockThread.suspend();
            }
         }
      }

   public void stop()
      {
      Trail("TestAppl: Stoping ");
      }

   public static void main(String args[])
      {
	  Trail("TestAppl: main");
      Frame f = new Frame();
      server srv = new server();
      srv.init();
      srv.start();

      f.add("Center", srv);
      f.setSize(300, 300);
      f.show();
      }
  }
