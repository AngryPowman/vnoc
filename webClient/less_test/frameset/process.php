<?php
	file_put_contents("../locations.csv",str_replace(",","",$_SERVER['HTTP_USER_AGENT']).",".$_GET['q']."\n",FILE_APPEND);
?>
<script>
	window.alert("лл���VNOC��֧�֣�\nлл���Ĳ��룬�������ת��VNOC��ҳ��");
	window.open('../../index.html','_self');
</script>