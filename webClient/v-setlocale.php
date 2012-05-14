<?php
	/**
	 ** v-setlocale.php
	 **
	 ** This is the locale setting file that set the locale to $_GET
	 ** value 'lang'
	 */
	require_once('v-load.php');
	
	v_locale_tocookie($_GET['lang']);
	v_locale_set();
	v_redirect(v_referer());
	exit();
?>