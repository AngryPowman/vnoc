<?
	/**
	 ** parse.php
	 **
	 ** Class: parse
	 ** This file corresponding to @_vnoc.parse class for v-load.php
	 ** file
	 */
	
	/** Parse for q-value (priority), mostly used in HTTP header parse
	 **
	 ** @_vnoc.parse
	 ** @param string $input - The entry string for parsing
	 ** @param string $config - The configuration of output and it is
	 **        determined by the following:
	 **        "v" for "Value"
	 **            If you want use value (or contents) as a index, use
	 **            this in the config string; in this case, "cascade"
	 **            may not work because the contents have to be unique
	 **        "p" for "Priority (q-value)":
	 **            If you want to use q-value as a index, use this in
	 **            the config string
	 **        Cation: If "p" and "v" get altogether it will include
	 **                both q-value and contents as indexes
	 **        "c" for "cascade":
	 **            If you want the output to cascade, use this in the
	 **            config string
	 ** @return array $parse - The browser language setting in array
	 */
	function v_parse_q($input, $config)
	{
		/** Get the boolean configurations */
		$is_qvalue=(stripos($config, "p") !== false);
		$is_value=(stripos($config, "v") !== false);
		$is_cascade=(stripos($config, "c") !== false);
		
		/** Split string into raw array (raw nodes with value =>
		 ** q-value)
		 */
		$arr_raw=preg_split("/\s*,\s*/", $input);
		foreach($arr_raw as $value)
		{
			$node=array();
			$node=preg_split("/\s*;\s*q\s*=\s*/", $value);
			/** If priority is not included, set it for 1 as default */
			if(count($node)==1)
				$node[]=1;
			$raw[$node[0]]=(float)$node[1];
		}
		
		/** Add value to cascaded nodes (if "v" in configuration) */
		if($is_value)
		{
			foreach($raw as $id => $value)
				$arr_cascade[$id][]=$value;
		}
		
		/** Add priority to cascaded nodes (if "p" in configuration) */
		if($is_qvalue)
		{
			foreach($raw as $id => $value)
				$arr_cascade[(string)$value][]=$id;
		}
		
		/** Select whether it is cascaded */
		switch($is_cascade)
		{
			/** It is cascade (with "c" configuration) */
			case true:
				return $arr_cascade;
			/** It is not cascade (without "c" configuration) */
			case false:
				foreach($arr_cascade as $id => $value)
					$arr_uncascade[$id]=implode(",", $value);
				return $arr_uncascade;
		}
	}
	
	/** Parse for User-Agent (mostly used in HTTP)
	 **
	 ** @_vnoc.parse
	 ** @param string $config
	 ** @return array $parse - The browser language setting in array
	 */
?>