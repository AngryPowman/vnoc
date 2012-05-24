<?
	function tool_mysql_connect($host=ini_get("mysql.default_host"), $user=ini_get("mysql.default_user"), $pass=ini_get("mysql.default_password"), $new_link=false, $client_flags=0)
	{
		return mysql_connect($host, $user, $pass, $new_link, $client_flag)
	}
	
	function tool_mysql_close()
	
	function tool_mysql_charset(optional $link)
	{
	}
?>