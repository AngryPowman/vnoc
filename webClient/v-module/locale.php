<?
	/**
	 ** locale.php
	 **
	 ** Class: locale
	 ** This file corresponding to @_vnoc.locale class for v-load.php
	 ** file
	 */
	
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
	 ** @return string $locale - Locale setting
	 */
	function v_locale()
	{
		if(isset($_COOKIE["vnoc_locale"]))
			return $_COOKIE["vnoc_locale"];
	}
	
	/** Set locale setting to cookie
	 ** If the language is not specific, it will use "en_US" as default
	 ** If you like to change the default, 
	 **
	 ** @_vnoc.locale
	 ** @param optional string $lang - Locale language
	 ** @return none
	 */
	function v_locale_tocookie($lang="en_US")
	{
		setcookie("vnoc_locale", $lang, time() + 31536000);
	}
	
	/** Clear locale setting from cookie
	 **
	 ** @_vnoc.locale
	 ** @param none
	 ** @return none
	 */
	function v_locale_clear()
	{
		$_COOKIE["vnoc_locale"]="";
	}
	 
	/** Set current locale setting using cookie value
	 **
	 ** @_vnoc.locale
	 ** @param none
	 ** @return none
	 */
	function v_locale_set()
	{
		if(!isset($_COOKIE["vnoc_locale"]))
			v_locale_tocookie();
		putenv("lang=" . v_locale());
		setlocale(LC_ALL, v_locale(), "utf-8");
		bindtextdomain(v_locale(), LANG);
		textdomain(v_locale());
	}
?>