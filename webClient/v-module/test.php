<?
	/**
	 ** test.php
	 **
	 ** Class: test
	 ** This file corresponding to @_vnoc.test class for v-load.php
	 ** file
	 */
	
	/** Get section name for LESS CSS tests
	 **
	 ** @_vnoc.test
	 ** @param none
	 ** @return string $section - The section name
	 */
	function v_section()
	{
		if(isset($_COOKIE["vnoc_section"]))
			return $_COOKIE["vnoc_section"];
	}
	
	/** Save section name to cookie (if specified)
	 ** If the cookie exists, the function call will replace it
	 ** This should come before any outputs on browser
	 ** This expires with browser session or die in v_section_clear()
	 ** function
	 **
	 ** @_vnoc.test
	 ** @param string $section - The section name for test
	 ** @return none
	 */
	function v_section_tocookie($section)
	{
		setcookie("vnoc_section", $section);
	}
	
	/** Clear section name from cookie cache
	 ** This commonly happenes when the form is submitted
	 **
	 ** @_vnoc.test
	 ** @param none
	 ** @return none
	 */
	function v_section_clear()
	{
		$_COOKIE["vnoc_section"]="";
	}
	
	/** Convert the options data from array to an integer
	 **
	 ** @_vnoc.test
	 ** @param array $source - The test value array, commonly from
	 **        $_POST value
	 ** @param optional string $prefix - Prefix for $source index (as an
	 **        indentifier), empty string as default
	 ** @param optional string $suffix - Suffix for $source index (as an
	 **        indentifier), empty string as default
	 ** @param optional bool $nocase - If it is true (default), it will
	 **        be regardless of the case of index (no matter is is upper
	 **        or lower cases)
	 ** @return int $integer - The integer output 
	 */
	function v_test_atoi($source, $prefix="", $suffix="", $nocase=true)
	{
		$integer=0;
		$count=0;
		foreach($source as $id => $value)
		{
			if(preg_match("/^($prefix)\s+($suffix)$/" . ($nocase ? "i" : ""), $id))
			{
				$integer=($integer << 1);
				$count+=1;
				if($value=="1")
					$integer+=1;
			}
		}
		return (($integer << 8) + $count);
	}
	
	/** Convert the options data from integer to an array of data
	 **
	 ** @_vnoc.test
	 ** @param int $integer - The integer to be converted
	 ** @return array $test - The test value array, starting from [0]
	 **  index
	 */
	function v_test_itoa($integer)
	{
		$arr=array();
		$count=($integer % 256);
		$integer=($integer >> 8);
		for($i=0;$i<$count;$i++)
		{
			$arr[]=($integer % 2);
			$integer=($integer >> 1);
		}
		$arr=array_reverse($arr);
		return $arr;
	}
?>