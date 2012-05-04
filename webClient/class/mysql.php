<?php
// CLASS - MySQL操作类;

function sql_connect($host,$user,$password,$db){
	$MySQL=mysql_connect($host,$user,$password) or die;
	mysql_select_db($db,$MySQL) or die;
	return $MySQL;
}
function sql_query($sql,$isfetch=true,$type=MYSQL_ASSOC){
	$MySQLConn=sql_connect("localhost","1025375_vnoc","vnoc","1025375_vnoc"); //fdb2.runhosting.com
	mysql_query("SET NAMES utf8") or die;
	$result=mysql_query($sql,$MySQLConn);
	if($isfetch)
		$result=mysql_fetch_array($result,$type);
	mysql_close($MySQLConn);
	return $result;
}
?>