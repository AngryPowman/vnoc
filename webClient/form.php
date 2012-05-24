<?php
	require_once("v-load.php");
	require_once("v-init.php");
	
// CLASS - MySQL操作类;

function sql_connect($host,$user,$password,$db){
	$MySQL=mysql_connect($host,$user,$password) or die;
	mysql_select_db($db,$MySQL) or die;
	return $MySQL;
}
function sql_query($sql,$isfetch=true,$type=MYSQL_ASSOC){
	$MySQLConn=sql_connect("fdb2.runhosting.com","1025375_vnoc","vnoc","1025375_vnoc"); //fdb2.runhosting.com
	mysql_query("SET NAMES utf8") or die;
	$result=mysql_query($sql,$MySQLConn);
	if($isfetch)
		$result=mysql_fetch_array($result,$type);
	mysql_close($MySQLConn);
	return $result;
}
?>
<?php
	if(isset($_GET["func"]) && $_GET["func"]=="post")
	{
		sql_query("INSERT INTO `info` (`user`, `passwd`, `email`, `signtime`) VALUES ('{$_POST["user"]}', '" . sha1($_POST["pass"]) . "', '{$_POST["email"]}', NOW())", false);
		$id=sql_query("SELECT `id` FROM `info` WHERE `user`='{$_POST["user"]}'");
		sql_query("INSERT INTO `meta` (`id`, `nick`, `gender`, `birthday`) VALUES ({$id["id"]}, '{$_POST["nick"]}', {$_POST["gender"]}, '{$_POST["year"]}-{$_POST["month"]}-{$_POST["day"]}')", false );
	}
?>

<!-- This is the header of HTML Code -->
<html>
<head><title>VNOC Homepage</title></head>
<body>
<form action="index.php?func=post" method="post">
<p><legend for="user">User</legend><input name="user" type="input"></input></p>
<p><legend for="pass">Pass</legend><input name="pass" type="password"></input></p>
<p><legend for="email">Email</legend><input name="email" type="input"></input></p>
<p><legend for="nick">Nick</legend><input name="nick" type="input"></input></p>
<p>
	<legend for="gender">Gender</legend>
	<select name="gender">
		<option value=1>Male
		<option value=2>Female
		<option value=0>Unknown
	</select>
</p>
<p>
<legend for="birth">Birthday</legend>
	<div name="birth">
		<select name="month">
			<?php
				for($i=1;$i<=12;$i++)
					printf("\t\t\t<option value=\"%d\">%d\n", $i, $i);
			?>
		</select>
		Month
		<select name="day">
			<?php
				for($i=1;$i<=31;$i++)
					printf("\t\t\t<option value=\"%d\">%d\n", $i, $i);
			?>
		</select>
		Day
		<select name="year">
			<?php
				for($i=1900;$i<=2090;$i++)
					printf("\t\t\t<option value=\"%d\">%d\n", $i, $i);
			?>
		</select>
		Year
	</div>
</p>
<p><input type="submit" value="Submit" /></p>
<!-- These are the cotents of HTML Code -->

</form>

<!-- This is the footer of HTML Code -->
</body>
</html>