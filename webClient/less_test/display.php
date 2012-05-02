<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN">
<html>
<meta http-equiv="Content-Type" content="text/html;charset=utf-16" />
<head>
	<title>LESS/CSS浏览器兼容性测试结果</title>
</head>
<body>
	<style>
		* {font-size:25px;font-family:georgia,黑体;}
		th {background-color:#E4E4E4;outline:black solid thin;}
		td {background-color:#D7E7FF;outline:black solid thin;}
	</style>
	<center>
		<p>LESS/CSS浏览器兼容性测试结果</p>
		<?php
			require_once("../class/mysql.php");
			require_once("sections.php");
			require_once("comments.php");
			foreach($section as $id => $value){
				echo "<p>【{$value}】测试结果</p>";
				echo "<table>";
				echo "	<tr>";
				echo "		<th>用户配置</th>";
				echo "		<th>测试时间</th>";
				$title=explode(" ",$comment[$id]);
				foreach($title as $titleval)
					echo "			<th>{$titleval}</th>";
				echo "	</tr>";
				foreach($rows as $val){
					echo "	<tr>";
					echo "		<td>{$row['setting']}</td>";
					echo "		<td>{$row['time']}</td>";
					echo "	</tr>";
				}
				echo "</table>";
			}
		?>
	</center>
</body>
</html>