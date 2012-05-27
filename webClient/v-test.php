<?php
	/**
	 ** v-test.php
	 **
	 ** This page is LESS CSS stylesheet test frame, which loads pages
	 ** in the folder v-test
	 */
	require_once("v-load.php");
	require_once("v-init.php");
	setcookie("vnoc_section", "locations");
?>

<!-- This is the head of HTML -->
<html>
<head>
	<title>
		<?php
			/** Output different titles for differenct function */
			switch($_GET["func"])
			{
				case "test":
					echo v_section() . __(" test");
					break;
				case "proc":
					echo __("Submitting Data ...");
					break;
				default:
					echo __("Test Summary");
			}
		?>
	</title>
</head>
<body>

<!-- This is the content -->
<?php
/** Select the function mode */
switch($_GET["func"])
{
	/** Test mode, show the form */
	case "test":
		/** This part is form header */
		echo "<form method=\"post\" action=\"v-test.php?func=proc\">\n";
		
		/** This part is loading data array */
		require_once(TEST . v_section() . ".inc.php");
		
		/** This part output the questions */
		foreach($question as $id => $q)
		{
			echo "\t<!-- This is the Question #{$id} -->\n";
			printf("\t<p>\n\t\t{$q}\n\t</p>\n", TEST . v_section() . ".php?" . $param[$id]);
			printf("\t<p>\n\t\t<input type=\"radio\" name=\"v-%s\" value=\"0\" checked />%s\n\t\t<input type=\"radio\" name=\"v-%s\" value=\"1\" />%s\n\t</p>\n", $id, __("Yes"), $id, __("No"));
			printf("\t<img>\n\t\t" . base64_decode($img[$id]) . "\n\t</img>\n");
			echo "\t\n";
		}
		
		/** This part is form footer */
		echo "<p>\n";
		echo "\t<input type=\"reset\" value=\"" . __("reset") . "\" />\n";
		echo "\t<input type=\"submit\" value=\"" . __("submit") . "\" />\n";
		echo "</p>\n";
		echo "</form>\n";
		break;
	/** Process mode, process the post data */
	case "proc":
		break;
	/** Default mode, show the test statistics */
	default:
}
?>

<!-- This is the footer of HTML -->
</body>
</html>