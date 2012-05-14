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
	 */
	define('ABSPATH', dirname(__FILE__) . '/');
	define('LESS'   , dirname(__FILE__) . '/v-less/');
	define('TEST'   , dirname(__FILE__) . '/v-test/');
	
	/** Initialize the LESS CSS JS Script
	 ** This must be declared after LESS CSS files
	 **
	 ** @_vnoc.script
	 ** @param none
	 ** @return script - Declare LESS CSS analyzer less.js file
	 */
	function v_LESS_init()
	{
		echo '<script src=\"' . LESS . 'less.js\" type=\"text/javascript\"></script>\n';
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
		echo '<link rel=\"stylesheet/less\" type=\"text/css\" href=\"{$less}\">\n';
	}
	
	/** Set the charset for the current page
	 **
	 ** @_vnoc.browser
	 ** @param string $charset - the charset, utf-8 as default
	 ** @return none
	 */
	function v_charset($charset)
	{
		header('Content-type: text/html; charset=' . $charset);
	}
	
	/** Set standard for the current page
	 **
	 ** @_vnoc.browser
	 ** @param string $std - Standard to use according to W3C standard
	 ** @return script - Declare <!DOCTYPE> and <html xmls>
	 */
	function v_standard($std)
	{
		echo '<!DOCTYPE {$std}>\n';
	}
	
	/** Redirect to a new URL
	 **
	 ** @_vnoc.browser
	 ** @param string $url - the url to go
	 ** @return none
	 */
	function v_redirect($url)
	{
		header('location:' . $url);
	}
	
	/** Get the referer to this page
	 **
	 ** @_vnoc.browser
	 ** @param none
	 ** @return string $referer - the referer url
	 */
	function v_referer()
	{
		return $_SERVER['HTTP_REFERER'];
	}
	
	/** An alias for gettext function
	 ** The funtion _ is predeclared by the system
	 ** To avoid redeclaring, use funtion __ instead
	 ** Just in case that system does note support aliases
	 **
	 ** @_vnoc.locale
	 ** @param string $text - Text to be gettexted
	 ** @return string $gettext - The gettexted text
	 */
	function __($text)
	{
		return gettext($text);
	}
	
	/** Get current locale setting
	 **
	 ** @_vnoc.locale
	 ** @param none
	 ** @return string $locale - locale setting
	 */
	function v_locale()
	{
		return $_COOKIE['vnoc_locale'];
	}
	
	/** Set locale setting to cookie
	 **
	 ** @_vnoc.locale
	 ** @param string $lang - locale language
	 ** @return none
	 */
	function v_locale_tocookie($lang)
	{
		setcookie('vnoc_locale', $lang, time() + 31536000);
	}
	 
	/** Clear locale setting from cookie
	 **
	 ** @_vnoc.locale
	 ** @param none
	 ** @return none
	 */
	function v_locale_clear()
	{
		$_COOKIE['vnoc_locale']='';
	}
	 
	/** Set current locale setting
	 ** If $_COOKIE['vnoc_locale'] is not set, use 'en_US' as default
	 ** If you'd like to change default, modify this value below
	 **
	 ** @_vnoc.locale
	 ** @param none
	 ** @return none
	 */
	function v_locale_set()
	{
		if(empty($_COOKIE['vnoc_locale']))
			v_locale_tocookie('zh_CN');
		setlocale(LC_ALL, $_COOKIE['vnoc_locale'], 'utf-8');
	}
?>