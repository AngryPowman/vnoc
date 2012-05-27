<?php
	/**
	 ** index.php
	 **
	 ** This is the index page of VNOC
	 */
	require_once("v-load.php");
	require_once("v-init.php");
?>

<!-- This is the header of HTML Code -->
<html>
<head><title><?php echo __("VNOC Homepage"); ?></title></head>
<body>

<!-- These are the cotents of HTML Code -->
<a href="v-setlocale.php?lang=en-US">|| American English ||</a>
<a href="v-setlocale.php?lang=en-GB">|| British English ||</a>
<a href="v-setlocale.php?lang=zh-HK">|| 港澳繁體 ||</a>
<a href="v-setlocale.php?lang=zh-TW">|| 台灣正體 ||</a>
<a href="v-setlocale.php?lang=zh-CN">|| 简体中文 ||</a>
<?php echo $_SERVER["HTTP_USER_AGENT"]; ?>
<p>This is [<?php echo __("VNOC Homepage"); ?>]</p>

<!-- This is the footer of HTML Code -->
</body>
</html>