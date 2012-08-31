#pragma once
typedef std::vector<CString> Strings;
struct BkDragDorp
{	
	POINT       pt;
	DWORD       dwDragOpt;
	DWORD       dwDragCtrl;
	DWORD		uCmdID;
	Strings     vStrings;
};
