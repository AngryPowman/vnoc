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
		echo "<!DOCTYPE {$std}>\n";
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
?>