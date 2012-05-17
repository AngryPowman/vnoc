<?php
	/**
	 ** v-test.php
	 **
	 ** This page is LESS CSS stylesheet test frame, which loads pages
	 ** in the folder v-test
	 */
	require_once("v-load.php");
	require_once("v-init.php");
?>

<!-- This is the head of HTML -->
<html>
<head><title><?php echo v_section() . __(" test"); ?></title></head>
<body>
<form method="post" action="v-test/_process.php">

<!-- This is the content -->
<?php
	/** This part is loading data array */
	require_once(TEST . v_section() . ".inc.php");
	
	/** This part output the questions */
	foreach($question as $id => $q)
	{
		printf("<p>\n\t" . $q . "\n</p>\n", v_section() . ".php?" . $param[$id]);
		printf("<p>\n\t<input type=\"radio\" name=\"v-%s\" value=\"0\" checked />%s\n\t<input type=\"radio\" name=\"v-%s\" value=\"1\" />%s\n</p>\n", $id, __("Yes"), $id, __("No"));
		printf("<p>" . base64_decode($img[$id]) . "</p>\n");
		printf("\n");
	}
?>

<!-- This is the footer of HTML -->
<p>
	<input type="reset" value="<?php echo __("reset") ?>" />
	<input type="submit" value="<?php echo __("submit"); ?>" />
</p>
</form>
</body>
</html>