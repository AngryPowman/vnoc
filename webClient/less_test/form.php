<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN">
<html>
<meta http-equiv="Content-Type" content="text/html;charset=utf-16" />
<head>
	<title><?php echo $_GET['section']; ?>.less测试</title>
</head>
<body>
	<style>
		body{text-align:center;}
		form{text-align:left;}
		form p{font-family:宋体;font-size:14pt;color:blue;}
		form img{border:blue solid 2pt;}
	</style>
	<?php
		require_once("{$_GET['section']}/contents.php");
		echo "<form method='post' action='process.php?section={$_GET['section']}&max={$len}'>";
		echo "	<input id=\"section\" type=\"hidden\" value=\"{$_GET['section']}\" />";
		foreach($questions as $id => $value){
			echo "		<p>{$value}</p>";
			echo "		<p>";
			echo "			<input type='radio' name='{$id}' value='0' checked />是";
			echo "			<input type='radio' name='{$id}' value='1' />否";
			echo "		</p>";
			echo "		<img src='{$_GET['section']}/{$imgs[$id]}' />";
		}
		echo "	<p>";
		echo "		<input type='submit' value='提交' />";
		echo "	</p>";
		echo "</form>";
	?>
</body>
</html>