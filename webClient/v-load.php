<?php
	/**
	 ** v-load.php
	 **
	 ** This is the load file for VNOC which include useful consts
	 ** and functions for VNOC.
	 */
	
	/** Define the environment paths for VNOC */
	define('ABSPATH', dirname(__FILE__) . '/');
	define('LESS'   , dirname(__FILE__) . '/v-less/');
	define('TEST'   , dirname(__FILE__) . '/v-test/');
	
	/** Initialize the LESS CSS JS Script
	 ** family: vnoc.script
	 ** params: none
	 ** return: script
	 */
	function v_LESS_init()
	{
		
	}
	
	/** Load *.less file to current position of code
	 ** family: vnoc.script
	 ** params: string $less - the LESS CSS to be loaded 
	 ** return: script
	 */
	function v_LESS_load($less)
	{
	
	}
	
	/** Set the charset for the current page
	 ** family: vnoc.browser
	 ** params: string $charset - the charset to go, utf-8 as default
	 ** return: none
	 */
	function v_charset($charset)
	{
		
	}
	
	/** Set standard for the current page
	 ** family: vnoc.browser
	 ** params: string $std - the standard you are using according to
	 **         W3C Standard
	 ** return: script
	 */
	function v_standard($std)
	{
		echo '<!DOCTYPE html>\n';
		echo '<html xmlns=\"http://www.w3.org/1999/xhtml\" {$std} />\n';
	}
	
	/** Redirect to a new URL
	 ** family: vnoc.browser
	 ** params: string $url - the url to go
	 ** return: none
	 */
	function v_redirect($url)
	{
		header('location:' . $url);
	}
	
	/** Get the referer to this page
	 ** family: vnoc.browser
	 ** params: none
	 ** return: string $referer - the referer url
	 */
	function v_referer()
	{
		return $_SERVER['HTTP_REFERER'];
	}
	
	/** Get current locale setting
	 ** family: vnoc.locale
	 ** params: none
	 ** return: string $locale - locale setting
	 */
	function v_locale()
	{
		return $_COOKIE['vnoc_locale'];
	}
	
	/** Set locale setting to cookie
	 ** family: vnoc.locale
	 ** params: string $lang - locale language
	 ** return: none
	 */
	 function v_locale_tocookie($lang)
	 {
		setcookie('vnoc_locale', $lang, time() + 31536000);
	 }
	 
	/** Set current locale setting
	 ** family: vnoc.locale
	 ** params: none
	 ** return: none
	 */
	function v_locale_set()
	{
		if(!isset($_COOKIE['vnoc_locale']))
			v_locale_tocookie('en_US');
		setlocale(LC_ALL, $_COOKIE['vnoc_locale'], 'utf-8');
	}
?>