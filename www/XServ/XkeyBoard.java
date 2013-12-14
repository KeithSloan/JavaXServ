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
import java.awt.event.*;

public class XkeyBoard 
   {
   private static final long serialVersionUID = 1L;
   Xsocket sock;
   

public XkeyBoard(Xsocket s)
   {
   sock = s;
   }

public void sendKeyEvent(int index,Rectangle r,Event e,int code,int detail,int state)
   {    
   sock.replyEvent(code);
   sock.addByte(detail);
   sock.addCard16(0);		// sequence number updated by javaServ
   // addCard32((int) e.when);	// time
   sock.addCard32(0);		// time  0 => Current
   sock.addCard32(0);		// root Window
   sock.addCard32(index);		// updated by javaServ
   sock.addCard32(0);		// Child
   sock.addCard16(r.x + e.x);
   sock.addCard16(r.y + e.y);
   sock.addCard16(e.x);
   sock.addCard16(e.y);
   sock.addCard16(state);		// State
   sock.addByte(1);		// Same Screen = true
   sock.addByte(0);		// unused 
   sock.flushEvent();
   }

public int keyMask(Event e)
    {
    int r = 0;
    if ( e.shiftDown())   r += 1;
    if ( e.controlDown()) r += 4; 
    return(r);
    }

public char KeySymToCode(int v)
   {
   char SYMtoCODE[] = {
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x00
          0,   0,   0,   0,   0,   0,  51,   0,   // 0x00
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x10
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x18
         69,  10,  11,  19,  13,  14,  15,  16,   // 0x20
         17,  18,  49,  48,  61,  20,  62,  63,   // 0x28
         19,  10,  11,  12,  13,  14,  15,  16,   // 0x30
         17,  18,  49,  48,  61,  20,  62,  63,   // 0x38
         35,  39,  58,  56,  41,  27,  42,  43,   // 0x40
         44,  32,  45,  46,  47,  60,  59,  33,   // 0x48
         34,  25,  28,  40,  29,  57,   0,  26,   // 0x50
         55,  30,  54,  36,   9,  50,  49,  53,   // 0x58
         35,  39,  58,  56,  41,  42,  43,  43,   // 0x60
         44,  32,  45,  46,  47,  60,  59,  33,   // 0x68
         34,  25,  28,  40,  29,  31,  57,  26,   // 0x70
         55,  30,  54,   0,  53,   0,  70,   0,   // 0x78
          0,   0,   0,  36,   0,   0,  36,   0,   // 0x80
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x88
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x90
          0,   0,   0,   0,   0,   0,   0,   0,   // 0x98
          0,   0,   0,  12,   0,   0,   0,   0,   // 0xa0
          0,   0,   0,   0,  21,   0,   0,   0,   // 0xa8
         19,  18,  11,  12,   0,   0,   0,   0,   // 0xb0
          0,   0,   0,   0,   0,  14,   0,   0,   // 0xb8
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xc0
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xc8
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xd0
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xd8
          0,  52,  65,  66,   0,   0,   0,   0,   // 0xe0
          0,  69,   0,   0,   0,   0,   0,  38,   // 0xe8
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xf0
          0,   0,   0,   0,   0,   0,   0,   0,   // 0xf8
        };

   return(SYMtoCODE[(int) v]);
   }
}
