<?php
	file_put_contents("../locations.csv",str_replace(",","",$_SERVER['HTTP_USER_AGENT']).",".$_POST['q1'].",".$_POST['q2'].",".$_POST['q3']."\n",FILE_APPEND);
?>
<script>
	window.alert("谢谢你对VNOC的支持！\n谢谢您的参与，浏览器将转到VNOC首页。");
	window.open('../../index.html','_self');
</script>