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

public class Trace
   {
   byte  traceFlag = 0;

   public Trace()
      {
      traceFlag = 0;
      }

   public void On(int v)
      {
      System.out.println("Trace On :"+v);
      traceFlag = (byte) v;
      }

   public void Entry(int flag,String text)
      {
      if (( traceFlag & (byte) flag) == (byte) flag)
         System.out.println(text);
      }

   private void printHex(int v)
      {
      char hexText[] = { '0','1','2','3','4','5','6','7',
                         '8','9','A','B','C','D','E','F' };
 
      v = v % 256;
      System.out.print("< "+hexText[v / 16]+hexText[v % 16]+" >");
      }

   public void Hex(int flag,int v)
      {
      if (( traceFlag & (byte) flag) == (byte) flag)
         {
         printHex(v);
         }
      }
   }
