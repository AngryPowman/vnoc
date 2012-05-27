<?
	/**
	 ** browser.php
	 **
	 ** Class: browser
	 ** This file corresponding to @_vnoc.browser class for v-load.php
	 ** file
	 */
	
	/** Set the charset for the current page
	 **
	 ** @_vnoc.browser
	 ** @param string $charset - The charset, utf-8 as default
	 ** @return none
	 */
	function v_charset($charset)
	{
		header("Content-type: text/html; charset=" . $charset);
	}
	
	/** Set standard for the current page
	 **
	 ** @_vnoc.browser
	 ** @param string $std - Standard to use according to W3C standard
	 ** @return script - Declare <!DOCTYPE> and <html xmls>
	 */
	function v_standard($std)
	{
		echo "<!DOCTYPE $std>\n";
	}
	
	/** Redirect to a new URL
	 **
	 ** @_vnoc.browser
	 ** @param string $url - The url to go
	 ** @return none
	 */
	function v_redirect($url)
	{
		header("location: " . $url);
	}
	
	/** Redirect after n second
	 **
	 ** @_vnoc.browser
	 ** @param string $url - The url to go
	 ** @param int $sec - The timer in second(s)
	 ** @return none
	 */
	function v_redirect_timer($url, $sec)
	{
		header("refresh:" . $sec . ";url=" . $url);
	}
	
	/** Refresh current page
	 **
	 ** @_vnoc.browser
	 ** @param none
	 ** @return none
	 */
	function v_refresh()
	{
		v_redirect(__FILE__);
	}
	
	/** Refresh after n second
	 **
	 ** @_vnoc.browser
	 ** @param int $sec - The timer in second(s)
	 ** @return none
	 */
	function v_refresh_timer($sec)
	{
		v_redirect_timer(__FILE__, $sec);
	}
	
	/** Goto previous page
	 **
	 ** @_vnoc.browser
	 ** @param none
	 ** @return none
	 */
	function v_back()
	{
		v_redirect(v_referer());
	}
	
	/** Goto previous page after n second(s)
	 **
	 ** @_vnoc.browser
	 ** @param int $sec - The timer in second(s)
	 ** @return none
	 */
	function v_back_timer($sec)
	{
		v_redirect_timer(v_referer(),$sec);
	}
	
	/** Get the referer to this page
	 **
	 ** @_vnoc.browser
	 ** @param none
	 ** @return string $referer - The referer url
	 */
	function v_referer()
	{
		if(isset($_SERVER["HTTP_REFERER"]))
			return $_SERVER["HTTP_REFERER"];
	}
	
	/** Get the browser name String
	 **
	 ** @_vnoc.browser
	 ** @param string $config - The configuration of output, please
	 **        refer to the function v_parse_UA()
	 ** @return mixed $browser - The browser name string
	 */
	function v_browser($config)
	{
		return $_SERVER["HTTP_USER_AGENT"];
	}
	
	/** Get the browser language setting
	 **
	 ** @_vnoc.browser
	 ** @param string $config - The configuration of output, please
	 **        refer to the function v_parse_q()
	 ** @return array $lang - The browser language setting in array
	 */
	function v_browser_lang($config)
	{
		return v_parse_q($_SERVER["HTTP_ACCEPT_LANGUAGE"], $config);
	}

	/** Get the browser charset setting
	 **
	 ** @_vnoc.browser
	 ** @param string $config - The configuration of output, please
	 **        refer to the function v_parse_q()
	 ** @return array $charset - The charset setting in array
	 */
	function v_browser_charset($config)
	{
		return v_parse_q($_SERVER["HTTP_ACCEPT_CHARSET"], $config);
	}
	
	/** Get the browser acceptable file setting
	 **
	 ** @_vnoc.browser
	 ** @param string $config - The configuration of output, please
	 **        refer to the function v_parse_q()
	 ** @return array $accept_file - The acceptable file setting array
	 */
	function v_browser_file($config)
	{
		return v_parse_q($_SERVER["HTTP_ACCEPT"], $config);
	}
?>