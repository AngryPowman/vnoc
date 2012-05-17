<?php
	/**
	 ** v-setlocale.php
	 **
	 ** This page works for locale setting
	 ** It set the locale and then redirect to the former page
	 */
	require_once("v-load.php");
	
	/** Save new locale setting to Cookies */
	v_locale_tocookie($_GET["lang"]);
	
	/** Set locale */
	v_locale_set();
	
	/** Redirect to former page */
	v_back();
?>