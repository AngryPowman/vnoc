<?php
	function getSystem(){
		$sys = $_SERVER['HTTP_USER_AGENT'];
		if(stripos($sys, "NT 6.1"))
			$os = "Windows 7";
		elseif(stripos($sys, "NT 6.0"))
			$os = "Windows Vista";
		elseif(stripos($sys, "NT 5.1"))
			$os = "Windows XP";
		elseif(stripos($sys, "NT 5.2"))
			$os = "Windows Server 2003";
		elseif(stripos($sys, "NT 5"))
			$os = "Windows 2000";
		elseif(stripos($sys, "NT 4.9"))
			$os = "Windows ME";
		elseif(stripos($sys, "NT 4"))
			$os = "Windows NT 4.0";
		elseif(stripos($sys, "98"))
			$os = "Windows 98";
		elseif(stripos($sys, "95"))
			$os = "Windows 95";
		elseif(stripos($sys, "Mac"))
			$os = "Mac";
		elseif(stripos($sys, "Linux"))
			$os = "Linux";
		elseif(stripos($sys, "Unix"))
			$os = "Unix";
		elseif(stripos($sys, "FreeBSD"))
			$os = "FreeBSD";
		elseif(stripos($sys, "SunOS"))
			$os = "SunOS";
		elseif(stripos($sys, "BeOS"))
			$os = "BeOS";
		elseif(stripos($sys, "OS/2"))
			$os = "OS/2";
		elseif(stripos($sys, "PC"))
			$os = "Macintosh";
		elseif(stripos($sys, "AIX"))
			$os = "AIX";
		else
			$os = "未知操作系统";
		return $os;
	}
	function getBrowser(){
		$sys = $_SERVER['HTTP_USER_AGENT'];
		if(stripos($sys, "NetCaptor") > 0)
			$exp = "NetCaptor";
		elseif(stripos($sys, "Firefox/") > 0){
			preg_match("/Firefox\/([^;)]+)+/i", $sys, $b);
			$exp = "Mozilla Firefox ".$b[1];
		}elseif(stripos($sys, "MAXTHON") > 0){
			preg_match("/MAXTHON\s+([^;)]+)+/i", $sys, $b);
			preg_match("/MSIE\s+([^;)]+)+/i", $sys, $ie);
			$exp = $b[0]." (IE".$ie[1].")";
		}elseif(stripos($sys, "MSIE") > 0){
			preg_match("/MSIE\s+([^;)]+)+/i", $sys, $ie);
			$exp = "Internet Explorer ".$ie[1];
		}elseif(stripos($sys, "Netscape") > 0)
			$exp = "Netscape";
		elseif(stripos($sys, "Opera") > 0)
			$exp = "Opera";
		else
			$exp = "未知浏览器";
		return $exp;
	}
?>