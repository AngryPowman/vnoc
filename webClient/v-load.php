<?php
	/**
	 ** v-load.php
	 **
	 ** This is the load file for VNOC which include useful consts
	 ** and functions for VNOC
	 */
	
	/** Define the environment paths for VNOC
	 **
	 ** @ABSPATH: the root path
	 ** @LESS:    the bunched LESS CSS file path
	 ** @TEST:    the contents for LESS CSS compatibility test
	 ** @LANG:    the language files
	 ** @MODULE:  the modules required for this "v-load.php" file
	 */
	define("ABSPATH", dirname(__FILE__) . "/");
	define("LESS"   , dirname(__FILE__) . "/v-less/");
	define("TEST"   , dirname(__FILE__) . "/v-test/");
	define("LANG"   , dirname(__FILE__) . "/v-lang/");
	define("MODULE" , dirname(__FILE__) . "/v-module/");
	
	/** Include in the modules needed for VNOC
	 **
	 ** @browser: Related with browser actions
	 ** @locale:  Related with locale settings
	 ** @mysql:   Functions with MySQL connection
	 ** @script:  Write out script with templates
	 ** @test:    Functions needed for LESS CSS compatibility test
	 */
	include_once(MODULE . "browser.php");
	include_once(MODULE . "locale.php");
	include_once(MODULE . "mysql.php");
	include_once(MODULE . "script.php");
	include_once(MODULE . "test.php");
?>