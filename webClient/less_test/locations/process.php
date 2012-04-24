<?php
	require_once("../get.php");
	file_put_contents("../locations.csv", getSystem().",".getBrowser().",".$_POST['q1'].",".$_POST['q2'].",".$_POST['q3']."\n", FILE_APPEND);
	echo "<script>alert(\"Ð»Ð»ÄãµÄºÏ×÷£¡\");</script>";
	echo "<script>location=\"../../index.html\";</script>";
?>