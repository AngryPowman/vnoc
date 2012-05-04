<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN">
<html>
<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
<head>
	<title>VNOC LESS/CSS 测试</title>
</head>
<body>
	<?php
		require_once("../class/mysql.php");
		$value=0;
		for($i=0;$i<$_GET['max'];$i++){
			if($_POST[$i]=="1"){
					$value=($value<<1)+1;
			}
		}
		$setting=str_replace(",","",$_SERVER['HTTP_USER_AGENT']);
		sql_query("INSERT INTO `lesstest` (`section`,`setting`,`value`,`time`) VALUES ('{$_GET['section']}','{$setting}',{$value},NOW())",false);
	?>
	<script>
		window.alert("谢谢你对VNOC的支持！\n谢谢您的参与，浏览器将转回到VNOC首页。");
		window.open('../index.php','_self');
	</script>
</body>
</html>