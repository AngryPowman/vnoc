<?
	/**
	 ** script.php
	 **
	 ** Class: script
	 ** This file corresponding to @_vnoc.script class for v-load.php
	 ** file
	 */
	
	/** Initialize the LESS CSS JS Script
	 ** This must be declared after LESS CSS files
	 **
	 ** @_vnoc.script
	 ** @param none
	 ** @return script - Declare LESS CSS analyzer less.js file
	 */
	function v_LESS_init()
	{
		echo "<script src=\"" . LESS . "less.js\" type=\"text/javascript\"></script>\n";
	}
	
	/** Load *.less file to current position of code
	 ** This must be declared before less.js
	 ** If there is not .less extension, it uses *.less as default
	 ** Otherwise, you have to specify *.css file
	 **
	 ** @_vnoc.script
	 ** @param string $less - The LESS CSS to be loaded 
	 ** @return script - Declare LESS CSS linking
	 */
	function v_LESS_load($less)
	{
		echo "<link rel=\"stylesheet/less\" type=\"text/css\" href=\"{$less}\">\n";
	}
?>