<?php
	/**
	 ** v-init.php
	 **
	 ** This file is to initialize the charset, locale and doctype for
	 ** common web pages.
	 */
	require_once("v-load.php");
	
	/** This part aim to set locale for the page */
	v_locale_set();
	
	/** This part aim to load header for PHP to set charset */
	v_charset("utf-8");
	
	/** This part aim to add script for the DOCTYPE Standard */
	v_standard("html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"");
?>