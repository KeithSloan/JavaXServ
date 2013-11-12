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

public class BasePanel extends Panel 
	{
	private static final long serialVersionUID = 1L;
	MapLayout ml;
        TextField userid;
        TextField password;
   
public BasePanel()
	{
	super();
	}     
    
private void Trail(String s)
	{
    System.out.println(s);
    }
   
public void init()
	{
	Trail("BasePanel Init");
//	setLayout(new GridLayout(2,2));

//	TextField userText = new TextField("   UserId",30);
//	userText.setEditable(new Boolean("FALSE"));
//	userid = new TextField("",30);
//	userid.setEditable(new Boolean("TRUE"));
//	TextField passText = new TextField("   Password",30);
//	passText.setEditable(new Boolean("FALSE"));
//	password = new TextField("",30);
//	password.setEchoChar('*');
//	password.setEditable(new Boolean("TRUE"));
	
//	add(userText);
//	add(userid);
//	add(passText);
//	add(password);
	
//	setVisible(new Boolean("TRUE"));
	}
}
