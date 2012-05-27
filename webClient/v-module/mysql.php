<?
	/**
	 ** mysql.php
	 **
	 ** Class: mysql
	 ** This file corresponding to @_vnoc.mysql class for v-load.php
	 ** file
	 */
	
	/** Connect to the mysql server
	 **
	 ** @_vnoc.mysql
	 ** @param optional string $host - The hostname (mysql Server)
	 ** @param optional string $user - The username
	 ** @param optional string $pass - The password
	 ** @param optional bool $new_link - Whether it is a new link
	 ** @param optional int $client_flag - The unique client flag
	 ** @return link $connection - The MySQL connection
	 */
	function v_mysql_connect(optional $host, optional $user, optional $pass, $new_link=false, $client_flags=0)
	{
		if(!isset($host))
			$host=ini_get("mysql.default_host");
		if(!isset($user))
			$user=ini_get("mysql.default_user");
		if(!isset($pass))
			$pass=ini_get("mysql.default_password");
		return mysql_connect($host, $user, $pass, $new_link, $client_flag);
	}
	
	/** Close a mysql connection
	 **
	 ** @_vnoc.mysql
	 ** @param optional link $link - The link identifier, previous as
	 **        default
	 ** @return bool $success - TRUE if succeed, FALSE if failure
	 */
	function v_mysql_close($link=NULL)
	{
		return mysql_close($link);
	}
	
	/** Retrieve the charset of the MySQL database
	 **
	 ** @_vnoc.mysql
	 ** @param optional link $link - The link identifier, previous as
	 **        default
	 ** @return string $charset - The MySQL charset
	 */
	function v_mysql_charset($link=NULL)
	{
		return mysql_client_encoding($link);
	}
	
	/** Get the string in a cell of result
	 **
	 ** @_vnoc.mysql
	 ** @param result $result - The result from retrieve
	 ** @param int $row - The row index
	 ** @param mixed $col - The column (filed) index (int) or name
	 **        (string)
	 ** @return string $data - The cell datum
	 */
	function v_mysql_error($partern, $col)
	{
		//return mysql_db_name($result, $row, $fieldx);
	}
?>