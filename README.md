JavaXServ (c) Keith Sloan

The idea behind JavaXServ is to allow people to run X11 Clients via a Java enabled browser.

The original motivation was to implement a facility such that IBM's AIX info application could be accssed from the internet via a browser.

Java security has changed a lot since the initial development and now requires a small amount of developer or two to address.

Normally to deploy X11 Clients an X Server has to be installed on the target machine. With JavaXServ the target machines only need to have a Java enabled browser.

In effect JavaXServ implements a three tier X11 system. In place of the normal X11 Client & X11 Server, with JavaXServ the X11 Server is split into two parts. A JavaXServ process written in C++ and XServ a Java Applet. A single JavaXServ process listens for multiple Java Applets, forking to pair each Java Applet with its X11 Client. JavaXServ tries to handle as much of the X11 protocol as possible and only passes onto the XServ Java applet what is needed.

Installation.

JavaXServ runs on a Unix or Linux system. Testing is done on an Ubuntu system.
The source for JavaXServ is available in the code section and can be downloaded by making the appropriate git request. Compilation is achived by running make.

At this stage of development one needs to start the JavaXServ process, but it is envisaged that inetd would be used when things reach a production level.

The webserver should have a static IP address
.
The directory www should be made available to a web server for example apache. For example via a soft symbolic link.

The two files config.php and configJar.php should be edited and the
correct IP address inserted.

For Java Security JavaXServ must run on the same machine as the Webserver where the Java applets are stored.

The end user needs access to a Java enabled browser. For testing I use Chromimum on a machine with Oracle Java(tm) SE installed see http://www.oracle.com/technetwork/java/javase/documentation/index.html

JavaXServ is an unsigned app.

My understanding is that you have to pay money to get a certificate to
create signed apps.

Anyway to run you need to set Security to Medium
You do this with the Java ControlPanel
which is normally in

   /usr/lib/jvm/<java version>/bin/ControlPanel


It is also useful to have the Java Console available for testing etc

