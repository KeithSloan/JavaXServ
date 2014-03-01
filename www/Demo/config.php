<!-- codebase points to machine and dirctory where Java Classes are server -->
<?php
echo '<applet code=server.class codebase="http://';
echo $_SERVER['SERVER_ADDR'];
echo '/XServ/classes" width=200 height=50>';
echo "\n";
//<--  server is machine where JavaXServ process is running ->
//<-- For Java Security this must be the same as is serving classes -->
echo '<param name="server" value="';
echo $_SERVER['SERVER_ADDR'];
echo '">';
echo "\n";
?>
<-- Port JavaXServ is listening on -->
<param name="port" value="6789"/>

