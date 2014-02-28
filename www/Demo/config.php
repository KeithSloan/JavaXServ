<!-- codebase points to machine and dirctory where Java Classes are server -->
<applet code=server.class codebase="http://192.168.1.3/XServ/classes" width=200 height=50>
<--  server is machine where JavaXServ process is running ->
<-- For Java Security this must be the same as is serving classes -->
<param name="server" value="192.168.1.3">
<-- Port JavaXServ is listening on -->
<param name="port" value="6789"/>

