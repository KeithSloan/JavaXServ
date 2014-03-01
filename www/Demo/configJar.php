<?php
echo '<applet code="server.class" codebase="http://';
echo $_SERVER['SERVER_ADDR'];
echo '/XServ" archive="JavaXServ.jar" width="200" height="50">';
echo "\n";
echo '<param name="server" value="';
echo $_SERVER['SERVER_ADDR'];
echo '"/>';
?>
<param name="port" value="6789"/>
<hr>
