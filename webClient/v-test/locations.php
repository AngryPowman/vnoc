<?php
	/**
	 ** locations.php
	 */
	require_once("../v-load.php");
	require_once("../v-init.php");
	
	/** This part load LESS CSS for demo */
	v_LESS_load(v_section() . ".less");
	v_LESS_init();
	
	/** This function gives the prefix for "class" property */
	function prefix()
	{
		switch($_GET["part"])
		{
			case "1":
				return "f";
				break;
			case "2":
				return "w";
				break;
			case "3":
				return "w";
				break;
			default:
				/** No Default for enum */
		}
	}
?>
<html>
<head><title><?php echo __("Noname"); ?></title></head>
<body>
	<div>
<?
	/** Output according to $_GET["part"] */
	if(($_GET["part"]=="1") || ($_GET["part"]=="2"))
	{
		printf("\t\t<span class=\"%s_t\">%s</span>\n", prefix(), __("Top 60px"));
		printf("\t\t<span class=\"%s_b\">%s</span>\n", prefix(), __("Bottom 60px"));
		printf("\t\t<span class=\"%s_l\">%s</span>\n", prefix(), __("Left 60px"));
		printf("\t\t<span class=\"%s_r\">%s</span>\n", prefix(), __("Right 60px"));
		printf("\t\t<span class=\"%s_hc\">%s</span>\n", prefix(), __("Centered Horizontally"));
		printf("\t\t<span class=\"%s_vc\">%s</span>\n", prefix(), __("Centered Vertically"));
		printf("\t\t<span class=\"%s_lt\">%s</span>\n", prefix(), __("Left-top 60px"));
		printf("\t\t<span class=\"%s_lc\">%s</span>\n", prefix(), __("Left 60px centered"));
		printf("\t\t<span class=\"%s_lb\">%s</span>\n", prefix(), __("Left-bottom 60px"));
		printf("\t\t<span class=\"%s_ct\">%s</span>\n", prefix(), __("Top 60px centered"));
		printf("\t\t<span class=\"%s_c\">%s</span>\n", prefix(), __("Centered"));
		printf("\t\t<span class=\"%s_cb\">%s</span>\n", prefix(), __("Bottom 60px centered"));
		printf("\t\t<span class=\"%s_rt\">%s</span>\n", prefix(), __("Right-top 60px"));
		printf("\t\t<span class=\"%s_rc\">%s</span>\n", prefix(), __("Right 60px centered"));
		printf("\t\t<span class=\"%s_rb\">%s</span>\n", prefix(), __("Right-bottom 60px"));
	}
	if($_GET["part"]=="1")
		echo "\t</div>\n\t<div>\n";
	if(($_GET["part"]=="1") || ($_GET["part"]=="3"))
	{
		printf("\t\t<span class=\"%s_h\">%s</span>\n", prefix(), __("Left And Right 60px"));
		printf("\t\t<span class=\"%s_v\">%s</span>\n", prefix(), __("Top And Bottom 60px"));
		printf("\t\t<span class=\"%s_s\">%s</span>\n", prefix(), __("Each edge 60px"));
	}
?>
	</div>
</body>
</html>